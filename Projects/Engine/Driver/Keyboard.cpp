
// Header
#include "Keyboard.h"

// Library includes

// Project includes

// Namespace declarations


bool isKeyDown(int keycode)
{
	int code = GetAsyncKeyState(keycode);

	if ( code & 0x8000 ) {
		code *= 1;
	}

	return (code & 0x8000) ? true : false;
}
