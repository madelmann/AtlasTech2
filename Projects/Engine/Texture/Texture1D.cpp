
// Header
#include "Texture1D.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace TextureSpace {


Texture1D::Texture1D()
{
	setTarget(GL_TEXTURE_1D);
}

Texture1D::~Texture1D()
{
}

AEint Texture1D::getWidth() const
{
	return mWidth;
}

void Texture1D::setWidth(AEint width)
{
	mWidth = width;
}

void Texture1D::unbind()
{
	glBindTexture(GL_TEXTURE_1D, NULL);
}

void Texture1D::uploadData(AEubyte *data)
{
	if ( !data ) {
		return;
	}

	glTexSubImage1D(GL_TEXTURE_2D, 0, 0, mWidth, mDataFormat, GL_UNSIGNED_BYTE, data);
}


}
