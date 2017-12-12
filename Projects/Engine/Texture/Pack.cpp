
// Header
#include "Pack.h"

// Library includes

// Project includes

// Namespace declarations


namespace TextureSpace {


TexturePack::TexturePack()
: mColor(0),
  mHeight(0),
  mNormal(0)
{
}

TexturePack::TexturePack(PackConfig config, Texture *color, Texture *height, Texture *normal)
: ATexturePack(config),
  mColor(color),
  mHeight(height),
  mNormal(normal)
{
}

Texture* TexturePack::color() const
{
	return mColor;
}

Texture* TexturePack::height() const
{
	return mHeight;
}

Texture* TexturePack::normal() const
{
	return mNormal;
}


}
