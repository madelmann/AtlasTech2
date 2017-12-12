
#ifndef Light_Types_h
#define Light_Types_h


// Library includes
#include <cassert>

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


namespace LightSpace {


class LightModel : public Common::ISettingsService
{
public:
	LightModel() {
		clear();
	}

public:
	void clear() {
		mGlobalAmbient = vector4f(0.2f, 0.2f, 0.2f, 1.0f);
		mLocalViewer = false;
		mTwoSided = false;
	}

	const vector4f& globalAmbient() const {
		return mGlobalAmbient;
	}
	void globalAmbient(const vector4f& global) {
		changed();
		mGlobalAmbient = global;
	}

	bool localViewer() const {
		return mLocalViewer;
	}
	void localViewer(bool state) {
		changed();
		mLocalViewer = state;
	}

	bool twoSided() const {
		return mTwoSided;
	}
	void twoSided(bool state) {
		changed();
		mTwoSided = state;
	}

private:
	vector4f	mGlobalAmbient;
	bool		mLocalViewer;
	bool		mTwoSided;
};


// Three modes for the light
class LightType
{
public:
	enum E {
		DirectionalLight = 0,
		PointLight,
		SpotLight
	};

	static E convert(int type) {
		switch ( type ) {
			case 0: return DirectionalLight;
			case 1: return PointLight;
			case 2: return SpotLight;
		}

		assert(!"invalid type provided!");
		return DirectionalLight;
	}
};


}


#endif
