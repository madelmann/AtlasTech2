
// Header
#include "FPSCounter.h"

// Library includes

// Project includes

// Namespace declarations


namespace Misc {


FPSCounter::FPSCounter()
: frameStart(0),
  frequency(0.0),
  mAllowHighFrequencyTimerUsage(false),
  mFrameRate(0.f),
  mFrameTime(0),
  mHighPerformanceTimerAvailable(false)
{
	init();
}

FPSCounter::~FPSCounter()
{
}

bool FPSCounter::allowHighFrequencyTimerUsage() const
{
	return mAllowHighFrequencyTimerUsage;
}

void FPSCounter::allowHighFrequencyTimerUsage(bool value)
{
	mAllowHighFrequencyTimerUsage = value;
}

float FPSCounter::getFrameRate() const
{
	return mFrameRate;
}

float FPSCounter::getFrameTime() const
{
	return mFrameTime;
}

void FPSCounter::init()
{
	mFrameEnd.HighPart = 0;
	mFrameEnd.LowPart = 0;
	mFrameEnd.QuadPart = 0;
	mFrameStart.HighPart = 0;
	mFrameStart.LowPart = 0;
	mFrameStart.QuadPart = 0;

	mHighPerformanceTimerAvailable = mTimer.highPerformanceTimerAvailable();
	if ( mHighPerformanceTimerAvailable ) {
		// get ticks per second
		QueryPerformanceFrequency(&mFrequency);

		frequency = (double)mFrequency.QuadPart;
	}
}

void FPSCounter::markFrameEnd()
{
	if ( mAllowHighFrequencyTimerUsage && mHighPerformanceTimerAvailable ) {
		QueryPerformanceCounter(&mFrameEnd);

		mFrameTime = float((mFrameEnd.QuadPart - mFrameStart.QuadPart) / frequency);
	}
	else {
		mFrameTime = float(mTimer.getCurrentTime() - frameStart);
	}

	mFrameRate = 1.f / mFrameTime;
}

void FPSCounter::markFrameStart()
{
	if ( mAllowHighFrequencyTimerUsage && mHighPerformanceTimerAvailable ) {
		QueryPerformanceCounter(&mFrameStart);
		return;
	}

	frameStart = mTimer.getCurrentTime();
}


}
