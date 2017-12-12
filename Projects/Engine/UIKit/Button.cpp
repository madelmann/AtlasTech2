
// Header
#include "Button.h"

// Library includes

// Project includes

// Namespace declarations


namespace UIKit {


Button::Button()
{
	mIsContainer = false;
}

void Button::render()
{
	float left = mLeft;
	float top = mTop;

	if ( mParent ) {
		left += mParent->getLeft();
		top += mParent->getTop();
	}

	UIFrame::render();

	aeColor(mColor);

	mFont->PrintText(mText, left, top);
}


}
