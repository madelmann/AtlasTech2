
// Header
#include "KeyGrabber.h"

// Library includes

// Project includes

// Namespace declarations


namespace Input {


KeyGrabber::KeyGrabber()
: mKey(0)
{
}

void KeyGrabber::bind(Key *k)
{
	mKey = k;
}

LRESULT KeyGrabber::handleEvent(const Common::Event &e)
{
	if ( mKey && e.uMsg == WM_KEYDOWN ) {
		mKey->setKeyCode(e.wParam);
		mKey = 0;

		// we have done all our work, so stop
		stop();
		return true;
	}

	return false;
}

void KeyGrabber::run()
{
	// nothing to do here
}


}
