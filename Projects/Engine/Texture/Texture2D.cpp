
// Header
#include "Texture2D.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace TextureSpace {


Texture2D::Texture2D()
{
	 setTarget(GL_TEXTURE_2D);
}

Texture2D::~Texture2D()
{
}

AEint Texture2D::getHeight() const
{
	return mHeight;
}

AEint Texture2D::getWidth() const
{
	return mWidth;
}

void Texture2D::setHeight(AEint height)
{
	mHeight = height;
}

void Texture2D::setWidth(AEint width)
{
	mWidth = width;
}

void Texture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture2D::uploadData(AEubyte *data)
{
	if ( !data ) {
		return;
	}

	// make sure we upload our data to the right texture
	bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
}


}
