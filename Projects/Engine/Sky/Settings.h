
#ifndef _Sky_Settings_h_
#define _Sky_Settings_h_


// Library includes
#include <string>

// Project includes
#include <Math/Vector3.h>
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace SkySpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		mAffectedByFog = false;
		mAffectedByLight = false;
		mFileFormat = "bmp";
		mPosition = vector3f();
		mSize = vector3f();
		mTheme = "";
		mUseFaceCulling = true;
	}

	bool affectedByFog() const {
		return mAffectedByFog;
	}
	void affectedByFog(bool state) {
		changed();
		mAffectedByFog = state;
	}

	bool affectedByLight() const {
		return mAffectedByLight;
	}
	void affectedByLight(bool state) {
		mAffectedByLight = state;
	}

	const std::string& fileFormat() const {
		return mFileFormat;
	}
	void fileFormat(const std::string& format) {
		if ( format.empty() ) {
			return;
		}
		mFileFormat = format;
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

	const std::string& theme() const {
		return mTheme;
	}
	void theme(const std::string& theme) {
		changed();
		mTheme = theme;
	}

	bool useFaceCulling() const {
		return mUseFaceCulling;
	}
	void useFaceCulling(bool state) {
		mUseFaceCulling = state;
	}

protected:

private:
	bool		mAffectedByFog;
	bool		mAffectedByLight;
	std::string	mFileFormat;
	vector3f	mPosition;
	vector3f	mSize;
	std::string	mTheme;
	bool		mUseFaceCulling;
};


}


#endif
