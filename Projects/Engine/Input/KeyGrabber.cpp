
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

void KeyGrabber::handleEvent(SDL_Event* e)
{
	if ( mKey && e->type == SDL_KEYDOWN ) {
		mKey->setKeyCode( reinterpret_cast<SDL_KeyboardEvent*>( e )->keysym.sym );
		// mKey = 0;

		// we have done all our work, so stop
		stop();
		return;
	}
}

void KeyGrabber::run()
{
	// nothing to do here
}


}
