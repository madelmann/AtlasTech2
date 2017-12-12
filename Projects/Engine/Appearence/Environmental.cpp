
// Header
#include "Environmental.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


Environmental::Environmental()
: IAttribute("Environmental")
{
}

void Environmental::bind()
{
	if ( !enabled() ) {
		return;
	}

	//glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		// Set The Texture Generation Mode For R To Sphere Mapping
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		// Set The Texture Generation Mode For S To Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		// Set The Texture Generation Mode For T To Sphere Mapping

	//aeEnable(GL_TEXTURE_GEN_R);									// Enable Texture Coord Generation For R
	aeEnable(GL_TEXTURE_GEN_S);									// Enable Texture Coord Generation For S
	aeEnable(GL_TEXTURE_GEN_T);									// Enable Texture Coord Generation For T
}

void Environmental::unbind()
{
	if ( !enabled() ) {
		return;
	}

	//aeDisable(GL_TEXTURE_GEN_R);								// Disable Texture Coord Generation For R
	aeDisable(GL_TEXTURE_GEN_S);								// Disable Texture Coord Generation For S
	aeDisable(GL_TEXTURE_GEN_T);								// Disable Texture Coord Generation For T
}
