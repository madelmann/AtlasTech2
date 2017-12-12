
// Header
#include "VI.h"

// Library includes
#include <assert.h>

// Project includes

// Namespace declarations


namespace Console {


VI::VI()
: IConsoleProgram("vi"),
  mMode(VisualMode)
{
}

VI::~VI()
{
}

void VI::processKeyPress(WPARAM key)
{
	switch ( key ) {
		case VK_ESCAPE:
			switchMode(VisualMode);
			return;
	}

	if ( mMode == InsertMode ) {
		switch ( key ) {
			case VK_BACK:
				return;
			case VK_DOWN:
				return;
			case VK_LEFT:
				return;
			case VK_RIGHT:
				return;
			case VK_UP:
				return;
		}
	}
	else if ( mMode == VisualMode ) {
		switch ( key ) {
			case VK_DOWN:
				return;
			case VK_LEFT:
				return;
			case VK_RIGHT:
				return;
			case VK_UP:
				return;
		}
	}
	else {
		// undefined mode
		assert(0);
	}
}

void VI::switchMode(eMode mode)
{
	mMode = mode;
}


}
