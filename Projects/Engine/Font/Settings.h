
#ifndef _Font_Settings_h_
#define _Font_Settings_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace FontSpace {


class Settings : public Common::ISettingsService
{
public:
	void clear() { }

	const std::string& getFontName() const {
		return mFontName;
	}
	void setFontName(const std::string& name) {
		changed();
		mFontName = name;
	}

	int getFontSize() const {
		return mFontSize;
	}
	void setFontSize(int size) {
		changed();
		mFontSize = size;
	}

protected:

private:
	float		mCharHeight;
	float		mCharWidth;
	std::string	mFontName;
	int			mFontSize;
};


}


#endif
