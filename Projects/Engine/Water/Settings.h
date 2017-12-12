
#ifndef _Water_Settings_h_
#define _Water_Settings_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Renderer/Common.h>

// Forward declarations

// Namespace declarations


namespace WaterSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		mColor = vector4f();
		mEnabled = false;
		mFacing = Core::Facing::Front;
		mFlowSpeed = vector2f();
		mNormals = false;
		mReflectionDepth = 0.f;
		mReflections = false;
		mRefractions = false;
		mPosition = vector3f();
		mSize = vector3f();
		mWaveHeight = 0.f;
		mWaveInterval = 0.f;
	}

	const vector4f& color() const {
		return mColor;
	}
	void color(const vector4f &color) {
		changed();
		mColor = color;
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool flag) {
		changed();
		mEnabled = flag;
	}

	Core::Facing::E facing() const {
		return mFacing;
	}
	void facing(Core::Facing::E facing) {
		changed();
		mFacing = facing;
	}

	const vector2f& flowSpeed() const {
		return mFlowSpeed;
	}
	void flowSpeed(const vector2f& speed) {
		changed();
		mFlowSpeed = speed;
	}

	const vector3f& position() const {
		return mPosition;
	}
	void position(const vector3f& position) {
		changed();
		mPosition = position;
	}

	float reflectionDepth() const {
		return mReflectionDepth;
	}
	void reflectionDepth(float depth) {
		changed();
		mReflectionDepth = depth;
	}

	const vector3f& size() const {
		return mSize;
	}
	void size(const vector3f& size) {
		changed();
		mSize = size;
	}

	float waveHeight() const {
		return mWaveHeight;
	}
	void waveHeight(float height) {
		changed();
		mWaveHeight = height;
	}

	float waveInterval() const {
		return mWaveInterval;
	}
	void waveInterval(float interval) {
		changed();
		mWaveInterval = interval;
	}

	bool useNormals() const {
		return mNormals;
	}
	void useNormals(bool state) {
		changed();
		mNormals = state;
	}

	bool useReflections() const {
		return mReflections;
	}
	void useReflections(bool state) {
		changed();
		mReflections = state;
	}

	bool useRefractions() const {
		return mRefractions;
	}
	void useRefractions(bool state) {
		changed();
		mRefractions = state;
	}

protected:

private:
	vector4f		mColor;
	bool			mEnabled;
	Core::Facing::E	mFacing;
	vector2f		mFlowSpeed;
	bool			mNormals;
	float			mReflectionDepth;
	bool			mReflections;
	bool			mRefractions;
	vector3f		mPosition;
	vector3f		mSize;
	float			mWaveHeight;
	float			mWaveInterval;
};


}


#endif
