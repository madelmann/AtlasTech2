
// Header
#include "Loader.h"

// Library includes

// Project includes
#include "TGA.h"
#include <Driver/GLWrapper.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace TextureSpace {


TgaLoader::TgaLoader(const Common::Logger *p)
: Common::Logger(p, "TgaLoader"),
  ATextureLoader("tga")
{
}

Texture* TgaLoader::create(const std::string &filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	TGAImg Img;

	// Load our Texture
	if ( Img.load(filename.c_str()) != IMG_OK ) {
		// Display an error message saying the file was not found, then return 0
		warn("file '" + filename + "' not found!\n");
		return 0;
	}

	Texture *texture = new Texture();
	texture->name(Tools::Files::ExtractFilename(filename));
	texture->setBPP(Img.getBPP());
	texture->setHeight(Img.getHeight());
	texture->mInternalFormat = texture->getBPP() / 8;
	texture->setWidth(Img.getWidth());


	glGenTextures(1, &texture->mDriverId);										// Generate a texture with the associative texture ID
	texture->bind();

	GLint alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);										// This sets the alignment requirements for the start of each pixel row in memory.

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);				// select modulate to mix texture with color for shading

	GLfloat wrap = 1.0f;
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				// when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, mEngineSettings->getAnsitropicFilter());

	switch ( texture->getBPP() ) {
		case 24:
			texture->mDataFormat = GL_RGB;

			if ( mEngineSettings->isCompressedTextures() ) {
				texture->mInternalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				texture->mIsCompressed = true;
			}
			break;
		case 32:
			texture->mDataFormat = GL_RGBA;

			if ( mEngineSettings->isCompressedTextures() ) {
				//texture->mInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				texture->mInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				//texture->mInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				texture->mIsCompressed = true;
			}
			break;
		default:
			assert(!"texture has an invalid bit count!");
			return 0;
			break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, texture->mInternalFormat, Img.getWidth(), Img.getHeight(), 0, texture->mDataFormat, GL_UNSIGNED_BYTE, Img.getImg());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &texture->mIsCompressed);

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	Img.~TGAImg();

	return texture;
}


}
