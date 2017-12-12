
// Header
#include "ScreenWriter.h"

// Library includes

// Project includes
#include <Font/Font.h>

// Namespace declarations


namespace Core {


ScreenWriter::ScreenWriter()
: mFont(0)
{
}

ScreenWriter::~ScreenWriter()
{
	shutdown();
}

void ScreenWriter::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mFont);

	mConfigurationComplete = true;
}

void ScreenWriter::connectFont(FontSpace::Font *f)
{
	mFont = f;
}

void ScreenWriter::render()
{
	for ( std::list<ScreenEntry*>::iterator it = mEntries.begin(); it != mEntries.end(); it++ ) {
		aeColor((*it)->mColor);

		mFont->PrintText((*it)->mText, (*it)->mX, (*it)->mY);
	}
}

void ScreenWriter::shutdown()
{
	for ( std::list<ScreenEntry*>::iterator it = mEntries.begin(); it != mEntries.end(); it++ ) {
		delete (*it);
	}

	mEntries.clear();
}

void ScreenWriter::update(float elapsedTime)
{
	std::list<ScreenEntry*>::iterator it = mEntries.begin();

	while ( it != mEntries.end() ) {
		(*it)->mTime -= (aeint)elapsedTime;

		if ( (*it)->mTime <= 0 ) {
			delete (*it);
			mEntries.erase(it++);
			continue;
		}

		it++;
	}
}

void ScreenWriter::write(float x, float y, const std::string& text, aeint time, const vector3f& color)
{
	if ( text.empty() ) {
		return;
	}

	mEntries.push_back(
		new ScreenWriter::ScreenEntry(x, y, text, time, color)
	);
}


}
