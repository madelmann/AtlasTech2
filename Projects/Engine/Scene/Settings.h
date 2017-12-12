
#ifndef _Scene_Settings_h_
#define _Scene_Settings_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Scene {


class Settings : public Common::ISettingsService
{
public:
	Settings()
	{
		clear();
	}

public:
	void clear() {
		mDayTime = 1200;
		mDistance_Fade = 0.f;
		mDistance_Hide = 0.f;
		mEditorMode = false;
	}

	float distanceFade() const {
		return mDistance_Fade;
	}
	void distanceFade(float fade) {
		mDistance_Fade = fade;
	}

	float distanceHide() const {
		return mDistance_Hide;
	}
	void distanceHide(float value) {
		mDistance_Hide = value;
	}

	int getDayTime() const {
		return mDayTime;
	}
	void setDayTime(int time) {
		changed();
		mDayTime = time;
	}

	bool getEditorMode() const {
		return mEditorMode;
	}
	void setEditorMode(bool state) {
		changed();
		mEditorMode = state;
	}

	bool* _getEditorMode() {
		return &mEditorMode;
	}

protected:

private:
	int		mDayTime;
	float	mDistance_Fade;
	float	mDistance_Hide;
	bool	mEditorMode;
};


}


#endif
