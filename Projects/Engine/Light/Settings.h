
#ifndef _Light_Setting_h_
#define _Light_Setting_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include "Types.h"

// Forward declarations

// Namespace declarations


namespace LightSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		mAmbient = vector4f();
		mAttenuation = vector3f(1.f, 0.0005f, 0.f);
		mCutOff = 180.0f;
		mDiffuse = vector4f();
		mDirection = vector3f();
		mEnabled = false;
		mExponent = 0.f;
		mMode = LightType::DirectionalLight;
		mPosition = vector3f();
		mSpecular = vector4f();
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool flag) {
		if ( mEnabled != flag ) {
			changed();
		}

		mEnabled = flag;
	}

	const vector4f& ambient() const {
		return mAmbient;
	}
	void ambient(const vector4f& ambient) {
		changed();
		mAmbient = ambient;
	}

	const vector3f& attenuation() const {
		return mAttenuation;
	}
	void attenuation(const vector3f& attenuation) {
		changed();
		mAttenuation = attenuation;
	}

	float constantAttenuation() const {
		return mAttenuation.x;
	}
	void constantAttenuation(float constant) {
		changed();
		mAttenuation.x = constant;
	}

	float cutOff() const {
		return mCutOff;
	}
	void cutOff(float cut) {
		changed();
		mCutOff = cut;
	}

	const vector4f& diffuse() const {
		return mDiffuse;
	}
	void diffuse(const vector4f& diffuse) {
		changed();
		mDiffuse = diffuse;
	}

	const vector3f& direction() const {
		return mDirection;
	}
	void direction(const vector3f& direction) {
		changed();
		mDirection = direction;
	}

	float exponent() const {
		return mExponent;
	}
	void exponent(float exponent) {
		changed();
		mExponent = exponent;
	}

	LightModel& lightModel() {
		return mLightModel;
	}

	float linearAttenuation() const {
		return mAttenuation.y;
	}
	void linearAttenuation(float linear) {
		changed();
		mAttenuation.y = linear;
	}

	LightType::E mode() const {
		return mMode;
	}
	void mode(LightType::E mode) {
		changed();
		mMode = mode;
	}

	const vector3f& position() const {
		return mPosition;
	}
	void position(const vector3f& position) {
		changed();
		mPosition = position;
	}

	float quadraticAttenuation() const {
		return mAttenuation.z;
	}
	void quadraticAttenuation(float quadric) {
		changed();
		mAttenuation.z = quadric;
	}

	const vector4f& specular() const {
		return mSpecular;
	}
	void specular(const vector4f& specular) {
		changed();
		mSpecular = specular;
	}

private:
	vector4f		mAmbient;
	vector3f		mAttenuation;
	float			mCutOff;
	vector4f		mDiffuse;
	vector3f		mDirection;
	bool			mEnabled;
	float			mExponent;
	LightModel		mLightModel;
	LightType::E	mMode;
	vector3f		mPosition;
	vector4f		mSpecular;
};


}


#endif
