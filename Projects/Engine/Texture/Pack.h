
#ifndef _Texture_Pack_h_
#define _Texture_Pack_h_


// Library includes
#include <string>

// Project includes
#include "Texture.h"
#include <Math/Vector2.h>

// Forward declarationbs

// Namespace declarations


namespace TextureSpace {


class PackConfig
{
public:
	PackConfig()
	: mAnimated(false)
	{ }

public:
	bool		mAnimated;
	std::string	mColor;
	std::string	mFormat;
	std::string	mHeight;
	std::string	mName;
	std::string	mNormal;
	vector2f	mSize;
};


class ATexturePack
{
public:
	ATexturePack() { }
	virtual ~ATexturePack() { }

public:
	PackConfig config() const {
		return mConfig;
	}
	std::string name() const {
		return mConfig.mName;
	}

protected:
	ATexturePack(PackConfig config)
	: mConfig(config)
	{ }

private:
	PackConfig	mConfig;
};


class TexturePack : public ATexturePack
{
public:
	TexturePack();
	TexturePack(PackConfig config, Texture *color, Texture *height, Texture *normal);

public:
	Texture* color() const;
	Texture* height() const;
	Texture* normal() const;

protected:

private:
	Texture	*mColor;
	Texture	*mHeight;
	Texture	*mNormal;
};


//class TexturePack1D : public ATexturePack
//{
//public:
//	TexturePack1D()
//	: mColor(0),
//	  mHeight(0),
//	  mNormal(0)
//	{ }
//	TexturePack1D(PackConfig config, Texture1D *color, Texture1D *height, Texture1D *normal)
//	: ATexturePack(config),
//	  mColor(color),
//	  mHeight(height),
//	  mNormal(normal)
//	{ }
//
//	Texture1D* color() const;
//	Texture1D* height() const;
//	Texture1D* normal() const;
//
//protected:
//
//private:
//	Texture1D	*mColor;
//	Texture1D	*mHeight;
//	Texture1D	*mNormal;
//};
//
//
//class TexturePack2D : public ATexturePack
//{
//public:
//	TexturePack2D()
//	: mColor(0),
//	  mHeight(0),
//	  mNormal(0)
//	{ }
//	TexturePack2D(PackConfig config, Texture2D *color, Texture2D *height, Texture2D *normal)
//	: ATexturePack(config),
//	  mColor(color),
//	  mHeight(height),
//	  mNormal(normal)
//	{ }
//
//	Texture2D* color() const;
//	Texture2D* height() const;
//	Texture2D* normal() const;
//
//protected:
//
//private:
//	Texture2D	*mColor;
//	Texture2D	*mHeight;
//	Texture2D	*mNormal;
//};
//
//
//class TexturePack3D : public ATexturePack
//{
//public:
//	TexturePack3D()
//	: mColor(0),
//	  mHeight(0),
//	  mNormal(0)
//	{ }
//	TexturePack3D(PackConfig config, Texture3D *color, Texture3D *height, Texture3D *normal)
//	: ATexturePack(config),
//	  mColor(color),
//	  mHeight(height),
//	  mNormal(normal)
//	{ }
//
//	Texture3D* color() const;
//	Texture3D* height() const;
//	Texture3D* normal() const;
//
//protected:
//
//private:
//	Texture3D	*mColor;
//	Texture3D	*mHeight;
//	Texture3D	*mNormal;
//};


}


#endif
