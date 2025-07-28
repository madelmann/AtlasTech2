
#ifndef _Misc_FPSCounter_h_
#define _Misc_FPSCounter_h_


// Library includes
#ifndef _WIN32
#include <chrono>
#endif

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
#ifdef _WIN32
	LARGE_INTEGER	mFrameEnd;
	LARGE_INTEGER	mFrameStart;
	LARGE_INTEGER	mFrequency;
#else
	std::chrono::high_resolution_clock::time_point	mFrameEnd;
	std::chrono::high_resolution_clock::time_point	mFrameStart;
	std::chrono::high_resolution_clock::time_point	mFrequency;
#endif

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
