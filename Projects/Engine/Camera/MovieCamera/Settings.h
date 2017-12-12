
#ifndef _Camera_MovieCamera_Settings_h_
#define _Camera_MovieCamera_Settings_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace CameraSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

	void clear() {
	}

protected:

private:

};


}


#endif
