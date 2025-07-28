
#ifndef _Sound_AudioResource_h_
#define _Sound_AudioResource_h_


// Library includes
#ifdef HAVE_OPENAL
#include <AL/al.h>
#include <AL/alc.h>
// ALUT is optional - comment out if not available
// #include <AL/alut.h>
// #include <AL/Framework.h>
#else
// OpenAL not available - provide dummy definitions
typedef int ALuint;
typedef int ALenum;
typedef float ALfloat;
#endif

// Project includes
#include <Interfaces/Core/Object.h>

// Forward declarations

// Namespace declarations


namespace SoundSpace {


struct AudioData
{
	AudioData()
	: mALBuffer(0),
	  mALBufferLen(0),
	  mALBufferData(0),
	  mALFormatBuffer(0),
	  mALFreqBuffer(0),
	  mALSource(0)
	{ }

	char*			mALBuffer;
	long			mALBufferLen;
	unsigned int	mALBufferData;
	ALenum			mALFormatBuffer;
	ALsizei			mALFreqBuffer;
	unsigned int	mALSource;
};


class AudioResource : public Core::Object
{
public:
	AudioResource(Core::AEid id = Core::invalidID, const std::string& name = "");

public:
	AudioData& data();
	void data(AudioData &data);

protected:

private:
	AudioData	mAudioData;
};


}


#endif
