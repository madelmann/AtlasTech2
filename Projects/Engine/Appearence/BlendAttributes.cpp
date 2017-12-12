
// Header
#include "BlendAttributes.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>


AEenum BlendAttributes::BlendingFactorDestination::convert(int value)
{
	switch ( value ) {
		case DST_ALPHA: return GL_DST_ALPHA;
		case ONE: return GL_ONE;
		case ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case SRC_ALPHA: return GL_SRC_ALPHA;
		case SRC_COLOR: return GL_SRC_COLOR;
		case ZERO: return GL_ZERO;
	}

	//assert(!"invalid value provided!");
	return GL_ZERO;
}

AEenum BlendAttributes::BlendingFactorDestination::convert(const std::string& value)
{
	if ( value == "DST_ALPHA" ) return GL_DST_ALPHA;
	if ( value == "ONE" ) return GL_ONE;
	if ( value == "ONE_MINUS_DST_ALPHA" ) return GL_ONE_MINUS_DST_ALPHA;
	if ( value == "ONE_MINUS_SRC_ALPHA" ) return GL_ONE_MINUS_SRC_ALPHA;
	if ( value == "ONE_MINUS_SRC_COLOR" ) return GL_ONE_MINUS_SRC_COLOR;
	if ( value == "SRC_ALPHA" ) return GL_SRC_ALPHA;
	if ( value == "SRC_COLOR" ) return GL_SRC_COLOR;
	if ( value == "ZERO" ) return GL_ZERO;

	//assert(!"invalid value provided!");
	return GL_ZERO;
}

AEenum BlendAttributes::BlendingFactorSource::convert(int value)
{
	switch ( value ) {
		case DST_ALPHA: return GL_DST_ALPHA;
		case DST_COLOR: return GL_DST_COLOR;
		case ONE: return GL_ONE;
		case ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case SRC_ALPHA: return GL_SRC_ALPHA;
		case SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
		case ZERO: return GL_ZERO;
	}

	//assert(!"invalid value provided!");
	return GL_ZERO;
}

AEenum BlendAttributes::BlendingFactorSource::convert(const std::string& value)
{
	if ( value == "DST_ALPHA" ) return GL_DST_ALPHA;
	if ( value == "ONE" ) return GL_ONE;
	if ( value == "ONE_MINUS_DST_ALPHA" ) return GL_ONE_MINUS_DST_ALPHA;
	if ( value == "ONE_MINUS_SRC_ALPHA" ) return GL_ONE_MINUS_SRC_ALPHA;
	if ( value == "ONE_MINUS_SRC_COLOR" ) return GL_ONE_MINUS_SRC_COLOR;
	if ( value == "SRC_ALPHA" ) return GL_SRC_ALPHA;
	if ( value == "SRC_ALPHA_SATURATE" ) return GL_SRC_ALPHA_SATURATE;
	if ( value == "SRC_COLOR" ) return GL_SRC_COLOR;
	if ( value == "ZERO" ) return GL_ZERO;

	//assert(!"invalid value provided!");
	return GL_ZERO;
}


BlendAttributes::BlendAttributes()
: IAttribute("BlendAttributes"),
  mDestination(GL_ZERO),
  mSource(GL_ZERO)
{
}

void BlendAttributes::bind()
{
	if ( !enabled() ) {
		return;
	}

	if ( mSource == GL_ZERO && mDestination == GL_ZERO ) {
		// Avoid calls to driver if not necessary
		return;
	}

    aeEnable(GL_BLEND);
    aeBlendFunc(mSource, mDestination);
}

AEenum BlendAttributes::destination() const
{
	return mDestination;
}

void BlendAttributes::destination(AEenum d)
{
	mDestination = d;//BlendingFactorDestination::convert(d);
}

void BlendAttributes::set(AEenum src, AEenum dest)
{
	destination(dest);
	source(src);
}

AEenum BlendAttributes::source() const
{
	return mSource;
}

void BlendAttributes::source(AEenum s)
{
	mSource = s;//BlendingFactorSource::convert(s);
}

void BlendAttributes::unbind()
{
	if ( mSource == GL_ZERO && mDestination == GL_ZERO ) {
		// Avoid calls to driver if not necessary
		return;
	}

    //aeBlendFunc(GL_ZERO, GL_ZERO);
	aeDisable(GL_BLEND);
}
