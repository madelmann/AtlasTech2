
// Header
#include "Basetexture.h"

// Library includes

// Project includes
#include <Driver\GLWrapper.h>

// Namespace declarations


namespace TextureSpace {


ABaseTexture::ABaseTexture(Core::AEid id, const std::string& name)
: Core::Object(id, name),
  mBpp(0),
  mDataFormat(GL_RGB),
  mDriverId(0),
  mEnvMode(0),
  mHeight(0),
  mInternalFormat(GL_RGB),
  mIsCompressed(false),
  mTarget(0),
  mWidth(0),
  mWrapModeR(0),
  mWrapModeS(0)
{
}

ABaseTexture::~ABaseTexture()
{
}

void ABaseTexture::bind()
{
	// binds texture to active texture channel

	glBindTexture(mTarget, mDriverId);
}

void ABaseTexture::bind(AEenum textureChannel)
{
	// sets texture channel to given one
	// and binds texture

	aeActiveTexture(textureChannel);

	bind();
}

AEuint ABaseTexture::getBPP() const
{
	return mBpp;
}

AEenum ABaseTexture::getTarget() const
{
	return mTarget;
}

void ABaseTexture::invalidate()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ABaseTexture::setBPP(AEuint bpp)
{
	mBpp = bpp;
}

void ABaseTexture::setTarget(AEuint target)
{
	mTarget = target;
}

void ABaseTexture::unbind()
{
	glBindTexture(mTarget, NULL);
}

void ABaseTexture::uploadData(AEubyte *)
{
	// implemented by derived classes
}


}
