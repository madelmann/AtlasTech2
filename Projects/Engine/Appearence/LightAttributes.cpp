
// Header
#include "LightAttributes.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


static vector4f white(1.f, 1.f, 1.f, 1.f);


AEenum LightAttributes::DrawBufferMode::convert(int value)
{
	switch ( value ) {
		case BACK: return GL_BACK;
		case FRONT: return GL_FRONT;
		case FRONT_AND_BACK: return GL_FRONT_AND_BACK;
	};

	assert(!"invalid value provided!");
	return 0;
}


LightAttributes::LightAttributes()
: IAttribute("LightAttributes"),
  mAmbient(1.f, 1.f, 1.f, 1.f),
  mDiffuse(1.f, 1.f, 1.f, 1.f),
  mDrawBufferMode(GL_FRONT_AND_BACK),
  mSpecular(1.f, 1.f, 1.f, 1.f)
{
}

vector4f LightAttributes::ambient() const
{
	return mAmbient;
}

void LightAttributes::ambient(const vector4f& ambient)
{
	mAmbient = ambient;
}

void LightAttributes::bind()
{
	aeMaterialfv(mDrawBufferMode, GL_AMBIENT,	mAmbient);
	aeMaterialfv(mDrawBufferMode, GL_DIFFUSE,	mDiffuse);
	aeMaterialfv(mDrawBufferMode, GL_SPECULAR,	mSpecular);
}

vector4f LightAttributes::diffuse() const
{
	return mDiffuse;
}

void LightAttributes::diffuse(const vector4f& diffuse)
{
	mDiffuse = diffuse;
}

AEenum LightAttributes::drawbuffermode() const
{
	return mDrawBufferMode;
}

void LightAttributes::drawbuffermode(AEenum m)
{
	mDrawBufferMode = DrawBufferMode::convert(m);
}

void LightAttributes::set(AEenum mode, const vector4f& a, const vector4f& d, const vector4f& s)
{
	ambient(a);
	diffuse(d);
	drawbuffermode(mode);
	specular(s);
}

vector4f LightAttributes::specular() const
{
	return mSpecular;
}

void LightAttributes::specular(const vector4f& specular)
{
	mSpecular = specular;
}

void LightAttributes::unbind()
{
	aeMaterialfv(mDrawBufferMode, GL_AMBIENT,	white);
	aeMaterialfv(mDrawBufferMode, GL_DIFFUSE,	white);
	aeMaterialfv(mDrawBufferMode, GL_SPECULAR,	white);
}
