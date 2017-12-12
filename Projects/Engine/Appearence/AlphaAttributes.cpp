
// Header
#include "AlphaAttributes.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>


AEenum AlphaAttributes::Function::convert(int value)
{
	switch ( value ) {
		case 0: return GL_NEVER;
		case 1: return GL_LESS;
		case 2: return GL_EQUAL;
		case 3: return GL_LEQUAL;
		case 4: return GL_GREATER;
		case 5: return GL_NOTEQUAL;
		case 6: return GL_GEQUAL;
		case 7: return GL_ALWAYS;
	}

	//assert(!"invalid value provided!");
	return GL_NEVER;
}

AEenum AlphaAttributes::Function::convert(const std::string& value)
{
	if ( value == "ALWAYS" ) return GL_ALWAYS;
	if ( value == "EQUAL" ) return GL_EQUAL;
	if ( value == "GEQUAL" ) return GL_GEQUAL;
	if ( value == "GREATER" ) return GL_GREATER;
	if ( value == "LEQUAL" ) return GL_LEQUAL;
	if ( value == "LESS" ) return GL_LESS;
	if ( value == "NEVER" ) return GL_NEVER;
	if ( value == "NOTEQUAL" ) return GL_NOTEQUAL;

	//assert(!"invalid value provided!");
	return GL_NEVER;
}


AlphaAttributes::AlphaAttributes()
: IAttribute("AlphaAttributes"),
  mFunction(GL_NEVER),
  mReference(1.f)
{
}

void AlphaAttributes::bind()
{
	if ( !enabled() ) {
		return;
	}
	if ( mFunction == GL_NEVER ) {
		// Avoid calls to driver if not necessary
		return;
	}

    aeEnable(GL_ALPHA_TEST);
    aeAlphaFunc(mFunction, mReference);
}

AEenum AlphaAttributes::function() const
{
	return mFunction;
}

void AlphaAttributes::function(AEenum f)
{
	mFunction = f;//Function::convert(f);
}

float AlphaAttributes::reference() const
{
	return mReference;
}

void AlphaAttributes::reference(float r)
{
	mReference = r;
}

void AlphaAttributes::set(AEenum f, float r)
{
	function(f);
	reference(r);
}

void AlphaAttributes::unbind()
{
	if ( !enabled() ) {
		return;
	}
	if ( mFunction == GL_NEVER ) {
		// Avoid calls to driver if not necessary
		return;
	}

    //aeAlphaFunc(GL_NEVER, 1.f);
	aeDisable(GL_ALPHA_TEST);
}
