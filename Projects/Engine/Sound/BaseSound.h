
#ifndef _Sound_BaseSound_h_
#define _Sound_BaseSound_h_


// Library includes

// Project includes
#include "AudioResource.h"
#include <Interfaces/Core/Object.h>

// Forward declarations

// Namespace decalarations


namespace SoundSpace {


class BaseSound : public Core::Object
{
public:
	virtual ~BaseSound();

public:
	void init();

	void pause();
	void play();
	void rewind();
	void stop();

	AudioResource* data();
	void data(AudioResource* data);

	float gain() const;
	void gain(float value);

	bool loopPlayBack() const;
	void loopPlayBack(bool state);

	float pitch() const;
	void pitch(float value);

	float progress() const;
	void progress(float progress);

protected:
	BaseSound(Core::AEid id = Core::invalidID, const std::string& name = "");

protected:
	AudioResource	*mResource;

private:
	float	mGain;
	bool	mInitialized;
	bool	mLoopPlayBack;
	float	mPitch;
	bool	mPlaying;
	float	mProgress;
};


}


#endif
