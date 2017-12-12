
// Header
#include "TranslationAttribute.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Scene {


TranslationAttribute::TranslationAttribute(const vector3f& value)
: mValue(value)
{
}

void TranslationAttribute::apply()
{
	aeTranslate(mValue);
}


}
