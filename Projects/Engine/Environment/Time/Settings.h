
#ifndef _Environment_Time_Settings_h_
#define _Environment_Time_Settings_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace Environment {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

	void clear() {
		changed();
	}

private:

};


}


#endif
