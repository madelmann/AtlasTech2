
// Header
#include "Loader.h"

// Library includes

// Project includes
#include "pcx.h"
#include <Driver/GLWrapper.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace TextureSpace {


PCXLoader::PCXLoader(const Common::Logger *p)
: Common::Logger(p, "PCXLoader"),
  ATextureLoader("pcx")
{
}

Texture* PCXLoader::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	struct pcx_header_t header;
	int bitcount;

	FILE *pFile = 0;

	/* Open image file */
	fopen_s(&pFile, filename.c_str(), "rb");
	if ( !pFile ) {
		warn("couldn't open '" + filename + "'!");
		return 0;
	}

	/* Read header file */
	fread(&header, sizeof (struct pcx_header_t), 1, pFile);
	if ( header.manufacturer != 0x0a ) {
		warn("bad version number!");
		return 0;
	}

	
	bitcount = header.bitsPerPixel * header.numColorPlanes;

	/* Initialize texture parameters */
	gl_texture_t *texinfo;
	texinfo = (struct gl_texture_t *)malloc(sizeof(struct gl_texture_t));
	texinfo->width = header.xmax - header.xmin + 1;
	texinfo->height = header.ymax - header.ymin + 1;
	texinfo->format = GL_RGB;
	texinfo->internalFormat = bitcount;
	texinfo->texels = (GLubyte *)malloc(sizeof(GLubyte) * texinfo->width * texinfo->height * texinfo->internalFormat);

	/* Read image data */
	switch ( bitcount ) {
		case 1:
			/* 1 bit color index */
			ReadPCX1bit(pFile, &header, texinfo);
			break;
		case 4:
			/* 4 bits color index */
			ReadPCX4bits(pFile, &header, texinfo);
			break;
		case 8:
			/* 8 bits color index */
			ReadPCX8bits(pFile, &header, texinfo);
			break;
		case 24:
			/* 24 bits */
			ReadPCX24bits(pFile, &header, texinfo);
			break;
		default:
			/* Unsupported */
			error("unknown bitcount pcx files");
			free(texinfo->texels);
			free(texinfo);
			texinfo = 0;
			break;
	}

	fclose(pFile);

	Texture *texture = new Texture();
	texture->name(Tools::Files::ExtractFilename(filename));
	texture->setBPP(texinfo->internalFormat * 8);
	texture->mDataFormat = texinfo->format;
	texture->setHeight(texinfo->height);
	texture->mInternalFormat = texinfo->internalFormat;
	if ( mEngineSettings->isCompressedTextures() ) {
		texture->mInternalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
	}
	texture->setWidth(texinfo->width);

	if ( !texinfo || !texinfo->texels ) {
		return 0;
	}

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

	glTexImage2D(GL_TEXTURE_2D, 0, texture->mInternalFormat, texture->getWidth(), texture->getHeight(), 0, texture->mDataFormat, GL_UNSIGNED_BYTE, texinfo->texels);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &texture->mIsCompressed);

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	free(texinfo->texels);
	free(texinfo);

	return texture;
}


}
