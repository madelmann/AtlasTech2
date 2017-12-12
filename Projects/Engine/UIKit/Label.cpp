
// Header
#include "Label.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Font/Font.h>

// Namespace declarations


namespace UIKit {


Label::Label()
{
}

void Label::render()
{
	float left = mLeft;
	float top = mTop;

	if ( mParent ) {
		left += mParent->getLeft();
		top += mParent->getTop();
	}

	aeColor(mColor);

	mFont->PrintText(mText, left, top);
}


}
