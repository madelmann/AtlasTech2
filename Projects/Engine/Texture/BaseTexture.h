
#ifndef _Texture_BaseTexture_h_
#define _Texture_BaseTexture_h_


// Library includes

// Project includes
#include <Interfaces/Core/Object.h>

// Forward declarations

// Namespace declarations


namespace TextureSpace {


class ABaseTexture : public Core::Object
{
public:
	ABaseTexture(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~ABaseTexture();

public:
	virtual void uploadData(AEubyte *data);

public:
	void bind();
	void bind(AEenum textureChannel);
	void invalidate();
	void unbind();

	AEuint getBPP() const;
	void setBPP(AEuint bpp);

	AEenum getTarget() const;
	void setTarget(AEenum type);

public:
	AEenum	mDataFormat;
	AEuint	mDriverId;
	AEuint	mEnvMode;
	AEenum	mInternalFormat;
	AEint	mIsCompressed;

protected:
	AEint	mHeight;
	AEint	mWidth;
	AEuint	mWrapModeR;
	AEuint	mWrapModeS;

private:
	// Copy operations are not allowed for textures, because
	// when the source texture is destroyed, it releases
	// its texture handle and so the destinations's texture
	// handle is not valid anymore.
	ABaseTexture (const ABaseTexture &);
	ABaseTexture &operator= (const ABaseTexture &);

private:
	AEuint	mBpp;
	AEenum	mTarget;
};


}


#endif
