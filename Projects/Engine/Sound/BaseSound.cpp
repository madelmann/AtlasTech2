
// Header
#include "BaseSound.h"

// Library includes

// Project includes

// Namespace declarations


namespace SoundSpace {


BaseSound::BaseSound(Core::AEid id, const std::string& name)
: Core::Object(id, name),
  mGain(1.f),
  mInitialized(false),
  mLoopPlayBack(false),
  mPitch(1.f),
  mPlaying(false),
  mProgress(0.f),
  mResource(0)
{
}

BaseSound::~BaseSound()
{
	if ( mResource ) {
		alDeleteSources(1, &mResource->data().mALSource);
		alDeleteBuffers(1, &mResource->data().mALBufferData);
	}
}

AudioResource* BaseSound::data()
{
	return mResource;
}

void BaseSound::data(AudioResource* data)
{
	mResource = data;
}

float BaseSound::gain() const
{
	return mGain;
}

void BaseSound::gain(float value)
{
	mGain = value;
}

void BaseSound::init()
{
	if ( mResource ) {
		//set the pitch
		alSourcef(mResource->data().mALSource, AL_PITCH, mPitch);
		//set the gain
		alSourcef(mResource->data().mALSource, AL_GAIN, mGain);
		//set looping to true
		alSourcei(mResource->data().mALSource, AL_LOOPING, mLoopPlayBack);
	}

	mInitialized = true;
}

bool BaseSound::loopPlayBack() const
{
	return mLoopPlayBack;
}

void BaseSound::loopPlayBack(bool state)
{
	mLoopPlayBack = state;
}

float BaseSound::pitch() const
{
	return mPitch;
}

void BaseSound::pitch(float value)
{
	mPitch = value;
}

void BaseSound::pause()
{
	mPlaying = false;

	alSourcePause(mResource->data().mALSource);
}

void BaseSound::play()
{
	mPlaying = true;

	alSourcePlay(mResource->data().mALSource);
}

float BaseSound::progress() const
{
	return mProgress;
}

void BaseSound::progress(float progress)
{
	mProgress = progress;
}

void BaseSound::rewind()
{
	alSourceRewind(mResource->data().mALSource);
}

void BaseSound::stop()
{
	mPlaying = false;
	mProgress = 0.f;

	alSourceStop(mResource->data().mALSource);
}


}
