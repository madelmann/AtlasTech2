
#ifndef ScriptNG_Settings_h
#define ScriptNG_Settings_h


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace ScriptSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		changed();

	}

private:

};


}


#endif
