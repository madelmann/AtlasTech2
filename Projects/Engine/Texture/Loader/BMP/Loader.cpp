
// Header
#include "Loader.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace TextureSpace {


BmpLoader::BmpLoader(const Common::Logger *p)
: Common::Logger(p, "BmpLoader"),
  ATextureLoader("bmp")
{
}

Texture* BmpLoader::create(const std::string &filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	// Load the bitmap using the aux function stored in glaux.lib
	AUX_RGBImageRec *pImage = auxDIBImageLoad(filename.c_str());

	// Make sure valid image data was given to pImage, otherwise return false
	if ( !pImage ) {
		warn("file '" + filename + "' not found!");
		return 0;
	}

	Texture *texture = new Texture();
	texture->name(Tools::Files::ExtractFilename(filename));
	texture->setBPP(24);
	texture->mDataFormat = GL_RGB;
	texture->setHeight(pImage->sizeY);
	texture->mInternalFormat = texture->getBPP() / 8;
	texture->setWidth(pImage->sizeX);


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

	glTexImage2D(GL_TEXTURE_2D, 0, texture->mInternalFormat, texture->getWidth(), texture->getHeight(), 0, texture->mDataFormat, GL_UNSIGNED_BYTE, pImage->data);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &texture->mIsCompressed);

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	// Now we need to free the image data that we loaded since openGL stored it as a texture
	if ( pImage ) {									// If we loaded the image
		if ( pImage->data ) {						// If there is texture data
			free(pImage->data);						// Free the texture data, we don't need it anymore
		}

		free(pImage);								// Free the image structure
	}

	return texture;
}


}
