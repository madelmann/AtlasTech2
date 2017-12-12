
// Header
#include "Timer.h"

// Library includes

// Project includes

// Namespace declarations


Timer::Timer()
: mCurrentTime(0.0),
  mHighPerformanceTimerAvailable(false)
{
	if ( QueryPerformanceFrequency(&mFrequency) ) {
		mHighPerformanceTimerAvailable = true;
	}
}

Timer::~Timer()
{
}

double Timer::getCurrentTime()
{
	if ( mHighPerformanceTimerAvailable ) {
		LARGE_INTEGER tmp;
		QueryPerformanceCounter(&tmp);

		mCurrentTime = static_cast<double>(tmp.QuadPart);
	}
	else {
		mCurrentTime = timeGetTime() * 0.001;
	}

	return mCurrentTime;
}

double Timer::getPrecision()
{
	if ( mHighPerformanceTimerAvailable ) {
		return static_cast<double>(mFrequency.QuadPart);
	}

	return 0;
}

double Timer::getElapsedTime(double lastTime)
{
	if ( mHighPerformanceTimerAvailable ) {
		return (getCurrentTime() - lastTime) / mFrequency.QuadPart;
	}

	return mCurrentTime - getCurrentTime();
}

bool Timer::highPerformanceTimerAvailable() const
{
	return mHighPerformanceTimerAvailable;
}
