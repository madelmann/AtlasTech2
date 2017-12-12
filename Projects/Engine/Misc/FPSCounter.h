
#ifndef _Misc_FPSCounter_h_
#define _Misc_FPSCounter_h_


// Library includes

// Project includes
#include "Timer.h"

// Forward declarations

// Namespace declarations


namespace Misc {


class FPSCounter
{
public:
    FPSCounter();
	~FPSCounter();

public:
	bool allowHighFrequencyTimerUsage() const;
	void allowHighFrequencyTimerUsage(bool value);

    float getFrameRate() const;
	float getFrameTime() const;

    void markFrameEnd();
    void markFrameStart();

protected:

private:
	void init();

private:
	LARGE_INTEGER	mFrameEnd;
	LARGE_INTEGER	mFrameStart;
	LARGE_INTEGER	mFrequency;

    double	frameStart;					// Mark the beginning of a frame
	double	frequency;

	bool	mAllowHighFrequencyTimerUsage;
    float	mFrameRate;					// The number of frames per second
    float	mFrameTime;					// The time since the frame began
	bool	mHighPerformanceTimerAvailable;
	Timer	mTimer;
};


}


#endif
