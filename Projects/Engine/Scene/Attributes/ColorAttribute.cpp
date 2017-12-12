
// Header
#include "ColorAttribute.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Scene {


ColorAttribute::ColorAttribute(const vector3f& value)
: mValue(value)
{
}

void ColorAttribute::apply()
{
	aeColor(mValue);
}


}
