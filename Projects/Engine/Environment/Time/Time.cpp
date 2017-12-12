
// Header
#include "Time.h"

// Library includes

// Project includes

// Namespace declarations


namespace Environment {


Time::Time()
: mTimeOfDay(0.5f)
{
}

Time::~Time()
{
	shutdown();
}

void Time::init()
{
}

void Time::manage()
{
	// nothing to do here
}

Settings* Time::provideSettings()
{
	return &mSettings;
}

void Time::shutdown()
{
}

float Time::timeOfDay() const
{
	return mTimeOfDay;
}

void Time::timeOfDay(float time)
{
	mTimeOfDay = time;
}

void Time::update(float elapsedTime)
{
	(void)elapsedTime;
}

}
