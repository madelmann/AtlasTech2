
// Header
#include "Texture3D.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace TextureSpace {


Texture3D::Texture3D()
: mDepth(0),
  mWrapModeT(0)
{
	setTarget(GL_TEXTURE_3D);
}

Texture3D::~Texture3D()
{
}

AEint Texture3D::getDepth() const
{
	return mDepth;
}

AEint Texture3D::getHeight() const
{
	return mHeight;
}

AEint Texture3D::getWidth() const
{
	return mWidth;
}

void Texture3D::setDepth(AEint depth)
{
	mDepth = depth;
}

void Texture3D::setHeight(AEint height)
{
	mHeight = height;
}

void Texture3D::setWidth(AEint width)
{
	mWidth = width;
}

void Texture3D::unbind()
{
	glBindTexture(GL_TEXTURE_3D, NULL);
}

void Texture3D::uploadData(AEubyte *)
{
	// to be implemented as soon as this is supported by OpenGL
}


}
