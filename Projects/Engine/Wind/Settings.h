
#ifndef _Wind_Settings_h_
#define _Wind_Settings_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace WindSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings()
	{
		clear();
	}

public:
	void clear() {
		mDirection = vector3f(0.f, 0.f, 0.f);
	}

	const vector3f& getDirection() const {
		return mDirection;
	}
	void setDirection(const vector3f& dir) {
		changed();
		mDirection = dir;
	}

protected:

private:
	vector3f	mDirection;
};


}


#endif
