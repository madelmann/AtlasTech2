
// Header
#include "Material.h"

// Library includes

// Project includes
#include <Texture/Texture.h>

// Namespace declarations


namespace MaterialSpace {


Material::Material(Core::AEid id, const std::string name)
: Core::Object(id, name),
  //IAttribute(name),
  mColorMap(0),
  mShininess(0.f)
{
}

void Material::bind()
{
	//enabled(true);

	mAlphaAttributes.bind();
	mBlendAttributes.bind();
	if ( mColorMap ) {
		mColorMap->bind();
	}
	mEnvironmentalAttributes.bind();
	mLightAttributes.bind();

	aeMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,	mEmissive);
	aeMaterialf (GL_FRONT_AND_BACK, GL_SHININESS,	mShininess);
}

void Material::unbind()
{
	mAlphaAttributes.unbind();
	mBlendAttributes.unbind();
	if ( mColorMap ) {
		mColorMap->unbind();
	}
	mEnvironmentalAttributes.unbind();
	mLightAttributes.unbind();

	vector4f clear;
	aeMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,	clear);
	aeMaterialf (GL_FRONT_AND_BACK, GL_SHININESS,	0.f);

	//enabled(false);
}


}
