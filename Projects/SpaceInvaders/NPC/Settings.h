
#ifndef NPC_Settings_h
#define NPC_Settings_h


// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace NPCSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		changed();

		mMovement = vector3f();
		mSpeed = 0.f;
	}

	const vector3f& movement() const {
		return mMovement;
	}
	void movement(const vector3f& value) {
		mMovement = value;
	}

	float speed() const {
		return mSpeed;
	}
	void speed(float value) {
		mSpeed = value;
	}

private:
	vector3f	mMovement;
	float		mSpeed;
};


}


#endif
