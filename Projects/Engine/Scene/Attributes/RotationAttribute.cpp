
// Header
#include "RotationAttribute.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Scene {


RotationAttribute::RotationAttribute(const vector3f& value)
: mValue(value)
{
}

void RotationAttribute::apply()
{
	aeRotate(mValue);
}


}
