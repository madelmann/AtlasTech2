
// Header
#include "TextFader.h"

// Library includes

// Project includes
#include "ScreenWriter.h"

// Namespace declarations


namespace Core {


TextFader::TextFader()
: mPaused(true),
  mProgress(0.f),
  mScreenWriter(0)
{
}

void TextFader::addLine(const std::string& line)
{
	mLines.push_back(line);
}

void TextFader::clear()
{
	mLines.clear();
	mPaused = true;
	mProgress = 0.f;
	mSettings = Settings();
}

void TextFader::connectScreenWriter(IScreenWriter *w)
{
	assert(w);
	assert(!mScreenWriter);

	mScreenWriter = w;
}

void TextFader::pause()
{
	mPaused = !mPaused;
}

void TextFader::render()
{
	for ( StringList::const_iterator it = mLines.begin(); it != mLines.end(); ++it ) {
		//mScreenWriter->write();
	}
}

const TextFader::Settings& TextFader::settings() const
{
	return mSettings;
}

void TextFader::settings(const Settings& s)
{
	mSettings = s;
}

void TextFader::start()
{
	mPaused = false;
}

void TextFader::stop()
{
	mPaused = true;
}


}
