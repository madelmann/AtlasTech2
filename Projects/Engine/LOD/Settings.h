
#ifndef _LOD_Settings_h_
#define _LOD_Settings_h_


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace LODSpace {


class Settings : public ISettingsService
{
public:
	Settings()
	: mMaterialDistance(0.f),
	  mShaderDistance(0.f),
	  mTextureDistance(0.f)
	{ }

	float getMaterialDistance() const {
		return mMaterialDistance;
	}
	void setMaterialDistance(float distance) {
		changed();
		mMaterialDistance = distance;
	}

	float getShaderDistance() const {
		return mShaderDistance;
	}
	void setShaderDistance(float distance) {
		changed();
		mShaderDistance = distance;
	}

	float getTextureDistance() const {
		return mTextureDistance;
	}
	void setTextureDistance(float distance) {
		changed();
		mTextureDistance = distance;
	}

protected:

private:
	float	mMaterialDistance;
	float	mShaderDistance;
	float	mTextureDistance;
};


}


#endif
