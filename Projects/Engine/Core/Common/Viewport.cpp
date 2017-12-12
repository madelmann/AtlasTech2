
// Header
#include "Viewport.h"

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Core {


Viewport::Viewport(AEint left, AEint top, AEint width, AEint height)
: mHeight(height),
  mLeft(left),
  mTop(top),
  mWidth(width)
{
}

void Viewport::set()
{
	aeViewport(mLeft, mTop, mWidth, mHeight);
}


}
