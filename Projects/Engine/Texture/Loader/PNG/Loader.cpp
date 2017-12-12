
// Header
#include "Loader.h"

// Library includes

// Project includes
#include "lodepng.h"
#include <Driver/GLWrapper.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace TextureSpace {


PngLoader::PngLoader(const Common::Logger *p)
: Common::Logger(p, "PngLoader"),
  ATextureLoader("png")
{
}

Texture* PngLoader::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	std::vector<unsigned char> image;
	lodepng::load_file(image, filename); //load the image file with given filename

	lodepng::State state;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, state, image);
	if ( error ) {
		warn("decoder error " + Tools::toString(error) + ": " + lodepng_error_text(error));
		return 0;
	}

	Texture *texture = new Texture();
	texture->name(Tools::Files::ExtractFilename(filename));
	texture->setBPP(state.info_png.color.bitdepth * (state.info_png.color.key_defined ? 4 : 3));
	texture->setHeight(height);
	texture->mInternalFormat = texture->getBPP() / 8;
	texture->setWidth(width);


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

	glTexImage2D(GL_TEXTURE_2D, 0, texture->mInternalFormat, width, height, 0, texture->mDataFormat, GL_UNSIGNED_BYTE, &image[0]);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &texture->mIsCompressed);

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	return texture;
}


}
