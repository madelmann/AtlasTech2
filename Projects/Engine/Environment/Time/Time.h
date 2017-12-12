
#ifndef _Environment_Time_Time_h_
#define _Environment_Time_Time_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Interfaces/Common/AManager.h>

// Forward declarations

// Namespace declarations


namespace Environment {


class Time : public Common::AManager
{
public:
	Time();
	~Time();

	// AManager implementation
	// {
	void clear();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	// }

	float timeOfDay() const;
	void timeOfDay(float time);

	Settings* provideSettings();

protected:

private:
	Settings	mSettings;

	float	mTimeOfDay;
};


}


#endif
