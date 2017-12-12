
#ifndef _Physics_Settings_h_
#define _Physics_Settings_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Physics {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		mDebug = false;
		mEnabled = false;
		mGravity = vector3f();
	}

	bool debugMode() const {
		return mDebug;
	}
	void debugMode(bool state) {
		changed();
		mDebug = state;
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool flag) {
		changed();
		mEnabled = flag;
	}

	const vector3f& gravity() const {
		return mGravity;
	}
	void gravity(const vector3f& gravity) {
		changed();
		mGravity = gravity;
	}

	const vector3f& position() const {
		return mPosition;
	}
	void position(const vector3f& position) {
		mPosition = position;
	}

	const vector3f& size() const {
		return mSize;
	}
	void size(const vector3f& size) {
		mSize = size;
	}

	const vector3f& subdivisions() const {
		return mSubdivisions;
	}
	void subdivisions(const vector3f& subdivisions) {
		mSubdivisions = subdivisions;
	}

protected:

private:
	bool		mDebug;
	bool		mEnabled;
	vector3f	mGravity;
	vector3f	mPosition;
	vector3f	mSize;
	vector3f	mSubdivisions;
};


}


#endif
