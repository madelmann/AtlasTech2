
#ifndef _Console_Settings_h_
#define _Console_Settings_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace Console {


class Settings : public Common::ISettingsService
{
public:
	Settings()
	: mFontName("Courier New")
	{
		clear();
	}

	void clear() {
		mBackground = true;
		mEcho = true;
		mEnabled = false;
		mFontSize = 20;
		mLineSpace = 0.05f;
		mMaxBufferedCommands = 10;
		mVisibleLines = 37;
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool state) {
		changed();
		mEnabled = state;
	}

	unsigned int maxBufferedCommands() const {
		return mMaxBufferedCommands;
	}
	void maxBufferedCommands(unsigned int max) {
		mMaxBufferedCommands = max;
	}

	bool getEcho() const {
		return mEcho;
	}
	void setEcho(bool state) {
		changed();
		mEcho = state;
	}

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

	float getLineSpace() const {
		return mLineSpace;
	}
	void setLineSpace(float space) {
		changed();
		mLineSpace = space;
	}

	unsigned int getVisibleLines() const {
		return mVisibleLines;
	}
	void setVisibleLines(unsigned int lines) {
		changed();
		mVisibleLines = lines;
	}

	void hideBackground() {
		mBackground = false;
	}
	bool isBackgroundVisible() const {
		return mBackground;
	}
	void showBackground() {
		changed();
		mBackground = true;
	}

protected:

private:
	bool			mBackground;
	bool			mEcho;
	bool			mEnabled;
	std::string		mFontName;
	int				mFontSize;
	float			mLineSpace;
	unsigned int	mMaxBufferedCommands;
	unsigned int	mVisibleLines;
};


}


#endif
