
#ifndef _Misc_Timer_h_
#define _Misc_Timer_h_


// Library includes
#ifndef _WIN32
#include <chrono>
#endif
// #include <windows.h>

// Project includes

// Forward declarations

// Namespace declarations


class Timer
{
public:
	Timer();
	~Timer();

public:
	/*
	*Read the system's current time and return it
	**/
	double getCurrentTime();

	/*
	*Return the timer's precision (frequency)
	**/
	double getPrecision();

	/*
	*Return the elapsed time in seconds
	**/
	double getElapsedTime(double lastTime);

	bool highPerformanceTimerAvailable() const;

protected:

private:
	double			mCurrentTime;
#ifdef _WIN32
	LARGE_INTEGER	mFrequency;
#else
	std::chrono::high_resolution_clock::time_point mStartTime;
#endif
	bool			mHighPerformanceTimerAvailable;
};


#endif
