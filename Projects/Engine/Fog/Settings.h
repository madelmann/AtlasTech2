
#ifndef Fog_Settings_h
#define Fog_Settings_h


// Library includes

// Project includes
#include "Types.h"
#include <Driver/GLWrapper.h>
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


namespace FogSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		changed();

		mAffectsSkyBox = false;
		mColor = vector4f();
		mDensity = 0.f;
		mEnabled = false;
		mEnd = 0.f;
		mMode = FogMode::Exponential;
		mStart = 0.f;
	}

	bool affectsSkyBox() const {
		return mAffectsSkyBox;
	}
	void affectsSkyBox(bool flag) {
		changed();
		mAffectsSkyBox = flag;
	} 

	const vector4f& color() const {
		return mColor;
	}
	void color(const vector4f& color) {
		changed();
		mColor = color;
	}

	float density() const {
		return mDensity;
	}
	void density(float density) {
		changed();
		mDensity = density;
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool flag) {
		changed();
		mEnabled = flag;
	}

	float end() const {
		return mEnd;
	}
	void end(float end) {
		changed();
		mEnd = end;
	}

	FogMode::E mode() const {
		return mMode;
	}
	void mode(FogMode::E mode) {
		changed();
		mMode = mode;
	}

	float start() const {
		return mStart;
	}
	void start(float start) {
		changed();
		mStart = start;
	}

private:
	bool		mAffectsSkyBox;
	vector4f	mColor;
	bool		mEnabled;
	float		mEnd;
	float		mDensity;
	FogMode::E	mMode;
	float		mStart;
};


}


#endif
