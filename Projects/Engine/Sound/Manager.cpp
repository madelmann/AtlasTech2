
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "AudioResource.h"
#include "Sound.h"
#include "Sound2D.h"
#include "Sound3D.h"
#include <Core/EngineSettings.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Tools/Files.h>
#include <Tools/Strings.h>

// Namespace declarations


namespace SoundSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "SoundSpace::Manager"),
  mEngineSettings(0),
  mMediaPathManager(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::clear()
{
	debug("Clearing...");

	for ( SoundList::iterator it = mSounds.begin(); it != mSounds.end(); ++it ) {
		delete (*it);
	}
	mSounds.clear();

	for ( ResourceList::iterator it = mAudioResources.begin(); it != mAudioResources.end(); ++it ) {
		delete (*it);
	}
	mAudioResources.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(mEngineSettings);
	assert(mMediaPathManager);
}

void Manager::connectEngineSettings(Core::EngineSettings *s)
{
	mEngineSettings = s;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

Sound* Manager::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	return create2D(Tools::toLowerCase(filename));
}

Sound* Manager::create2D(const std::string& filename)
{
	debug("create2D('" + filename + "')");

	if ( filename.empty() ) {
		warn("create2D: invalid filename provided - aborting...");
		return 0;
	}

	Sound *sound = find(filename);
	if ( sound ) {
		return sound;
	}

	sound = new Sound2D();

	bool result = createAudioResource(filename, sound);
	if ( !result ) {
		error("error while creating audio resource - aborting...");
		delete sound;
		return 0;
	}

	mSounds.push_back(sound);

	return sound;
}

Sound* Manager::create3D(const std::string& filename)
{
	debug("create3D('" + filename + "')");

	if ( filename.empty() ) {
		warn("create3D: invalid filename provided - aborting...");
		return 0;
	}

	Sound *sound = find(filename);
	if ( sound ) {
		return sound;
	}

	sound = new Sound3D();

	bool result = createAudioResource(filename, sound);
	if ( !result ) {
		error("error while creating audio resource - aborting...");
		delete sound;
		return 0;
	}

	mSounds.push_back(sound);

	return sound;
}

bool Manager::createAudioResource(const std::string& filename, Sound *sound)
{
	debug("createAudioResource('" + filename + "')");

	if ( filename.empty() ) {
		warn("createAudioResource: invalid filename provided - aborting...");
		return false;
	}

	AudioData data;
	AudioResource *resource = new AudioResource();

	std::string file = mMediaPathManager->buildAudioPath(filename);

	// load sound data...
	ALboolean loop = false;
	char fname[256];
	sprintf(fname, "%s", file.c_str());

	alutLoadWAVFile(fname, &data.mALFormatBuffer, (void **)&data.mALBuffer, (ALsizei *)&data.mALBufferLen, &data.mALFreqBuffer, &loop);

	resource->data(data);
	mAudioResources.push_back(resource);

	alGenSources(1, &data.mALSource);
	alGenBuffers(1, &data.mALBufferData);
	alBufferData(data.mALBufferData, data.mALFormatBuffer, data.mALBuffer, data.mALBufferLen, data.mALFreqBuffer);
	alSourcei(data.mALSource, AL_BUFFER, data.mALBufferData);

	alutUnloadWAV(data.mALFormatBuffer, data.mALBuffer, data.mALBufferLen, data.mALFreqBuffer);			

	// assign the created audio resource
	sound->data(resource);

	sound->init();

	return true;
}

Sound* Manager::find(const std::string& name) const
{
	for ( SoundList::const_iterator it = mSounds.begin(); it != mSounds.end(); ++it ) {
		if ( name.compare((*it)->name()) ) {
			return (*it);
		}
	}

	return 0;
}

void Manager::init()
{
	debug("Initializing...");

	// Initialize OpenAL
	mDevice = alcOpenDevice(NULL); // open default device
	if ( !mDevice ) {
		//fatal("Invalid OpenAL Device!");
		error("Invalid OpenAL Device!");
		return;
	}

	mContext = alcCreateContext(mDevice, NULL); // create context
	if ( !mContext ) {
		//fatal("Invalid OpenAL Context!");
		error("Invalid OpenAL Context!");
		return;
	}

	alcMakeContextCurrent(mContext); // set active context

	ALenum alError = alGetError();
	ALenum alcError = alcGetError(mDevice);
	ALenum alutError = alutGetError();

	if ( alError != AL_NO_ERROR || alcError != ALC_NO_ERROR || alutError != ALUT_ERROR_NO_ERROR ) {
		char errstr[80] = "";

		switch ( alError ) {
			case AL_NO_ERROR: /*this is no error*/ break;
			case AL_INVALID_NAME: sprintf(errstr, "AL_INVALID_NAME"); break;
			//case AL_ILLEGAL_ENUM: sprintf(errstr, "AL_ILLEGAL_ENUM"); break;
			case AL_INVALID_ENUM: sprintf(errstr, "AL_INVALID_ENUM"); break;
			case AL_INVALID_VALUE: sprintf(errstr, "AL_INVALID_VALUE"); break;
			//case AL_ILLEGAL_COMMAND: sprintf(errstr, "AL_ILLEGAL_COMMAND"); break;
			case AL_INVALID_OPERATION: sprintf(errstr, "AL_INVALID_OPERATION"); break;
			case AL_OUT_OF_MEMORY: sprintf(errstr, "AL_OUT_OF_MEMORY"); break;
		}

		switch ( alcError ) {
			case ALC_NO_ERROR: /*this is no error*/ break;
			case ALC_INVALID_DEVICE: sprintf(errstr, "ALC_INVALID_DEVICE"); break;
			case ALC_INVALID_CONTEXT: sprintf(errstr, "ALC_INVALID_CONTEXT"); break;
			case ALC_INVALID_ENUM: sprintf(errstr, "ALC_INVALID_ENUM"); break;
			case ALC_INVALID_VALUE: sprintf(errstr, "ALC_INVALID_VALUE"); break;
			case ALC_OUT_OF_MEMORY: sprintf(errstr, "ALC_OUT_OF_MEMORY"); break;
		}

		switch ( alutError ) {
			case ALUT_ERROR_NO_ERROR: /*this is no error*/ break;
			case ALUT_ERROR_OUT_OF_MEMORY: sprintf(errstr, "ALUT_ERROR_OUT_OF_MEMORY"); break;
			case ALUT_ERROR_INVALID_OPERATION: sprintf(errstr, "ALUT_ERROR_INVALID_OPERATION"); break;
			case ALUT_ERROR_NO_CURRENT_CONTEXT: sprintf(errstr, "ALUT_ERROR_NO_CURRENT_CONTEXT"); break;
			case ALUT_ERROR_AL_ERROR_ON_ENTRY: sprintf(errstr, "ALUT_ERROR_AL_ERROR_ON_ENTRY"); break;
			case ALUT_ERROR_ALC_ERROR_ON_ENTRY: sprintf(errstr, "ALUT_ERROR_ALC_ERROR_ON_ENTRY"); break;
			case ALUT_ERROR_GEN_BUFFERS: sprintf(errstr, "ALUT_ERROR_GEN_BUFFERS"); break;
			case ALUT_ERROR_BUFFER_DATA: sprintf(errstr, "ALUT_ERROR_BUFFER_DATA"); break;
			case ALUT_ERROR_IO_ERROR: sprintf(errstr, "ALUT_ERROR_IO_ERROR"); break;
			case ALUT_ERROR_UNSUPPORTED_FILE_TYPE: sprintf(errstr, "ALUT_ERROR_UNSUPPORTED_FILE_TYPE"); break;
			case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE: sprintf(errstr, "ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE"); break;
			case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA: sprintf(errstr, "ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA"); break;
		}

		error("OpenAL error: " + std::string(errstr));
	}

	// Clear Error Code
	alGetError();
	alcGetError(mDevice);
	alutGetError();
}

void Manager::manage()
{
	for ( SoundList::const_iterator it = mSounds.begin(); it != mSounds.end(); ++it ) {
		Sound *s = (*it);
		//AudioResource *res = s->data();

		if ( s->progress() >= 100.f ) {
			if ( s->loopPlayBack() ) {
				s->progress(0.f);
				s->rewind();
			}
			else {
				s->stop();
			}
		}
	}
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float /*elapsedTime*/)
{
}


}
