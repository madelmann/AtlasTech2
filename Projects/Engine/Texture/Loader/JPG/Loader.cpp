
// Header
#include "Loader.h"

// Library includes

// Project includes
#include "jpeg.h"
#include <Driver/GLWrapper.h>
#include <JPEG/includes/jpeglib.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace TextureSpace {


JPGLoader::JPGLoader(const Common::Logger *p)
: Common::Logger(p, "JPGLoader"),
  ATextureLoader("jpg")
{
}

Texture* JPGLoader::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	struct jpeg_decompress_struct cinfo;
	tImage *pImage = 0;

	FILE *pFile = 0;

	/* Open image file */
	fopen_s(&pFile, filename.c_str(), "rb");
	if ( !pFile ) {
		warn("couldn't open '" + filename + "'!");
		return 0;
	}

	// Create an error handler
	jpeg_error_mgr jerr;

	// Have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);

	// Initialize the decompression object
	jpeg_create_decompress(&cinfo);

	// Specify the data source (Our file pointer)	
	jpeg_stdio_src(&cinfo, pFile);

	// Allocate the structure that will hold our eventual jpeg data (must free it!)
	pImage = new tImage();

	// Decode the jpeg file and fill in the image data structure to pass back
	decode(&cinfo, pImage);

	// This releases all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&cinfo);

	// Close the file pointer that opened the file
	fclose(pFile);

	// Make sure valid image data was given to pImage, otherwise return false
	if ( pImage->data == 0 ) {
		delete pImage;
		return 0;
	}

	Texture* texture = new Texture();
	texture->name(Tools::Files::ExtractFilename(filename));
	texture->setBPP(pImage->channels * 8);
	texture->setHeight(pImage->sizeY);
	texture->mInternalFormat = pImage->channels;
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
		case 8:
			texture->mDataFormat = GL_DEPTH_COMPONENT;

			if ( mEngineSettings->isCompressedTextures() ) {
				texture->mInternalFormat = GL_COMPRESSED_ALPHA;
				texture->mIsCompressed = true;
			}
			break;
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

	glTexImage2D(GL_TEXTURE_2D, 0, texture->mInternalFormat, pImage->sizeX, pImage->sizeY, 0, texture->mDataFormat, GL_UNSIGNED_BYTE, pImage->data);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &texture->mIsCompressed);

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	if ( pImage->data ) {
		free(pImage->data);
	}
	delete pImage;

	return texture;
}

void JPGLoader::decode(jpeg_decompress_struct* cinfo, tImage *pImageData)
{
	// Read in the header of the jpeg file
	jpeg_read_header(cinfo, TRUE);

	// Start to decompress the jpeg file with our compression info
	jpeg_start_decompress(cinfo);

	// Get the image dimensions and channels to read in the pixel data
	pImageData->channels = cinfo->num_components;
	pImageData->sizeX    = cinfo->image_width;
	pImageData->sizeY    = cinfo->image_height;

	// Get the row span in bytes for each row
	int rowSpan = cinfo->image_width * cinfo->num_components;

	// Allocate memory for the pixel buffer
	pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*rowSpan*pImageData->sizeY));

	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[pImageData->sizeY];

	for ( int i = 0; i < pImageData->sizeY; i += 1 ) {
		// assign row pointer upside down so the jpeg images gets stored correctly in memory
		rowPtr[pImageData->sizeY - i - 1] = &(pImageData->data[i * rowSpan]);
	}

	// Now comes the juice of our work, here we extract all the pixel data
	int rowsRead = 0;
	while ( cinfo->output_scanline < cinfo->output_height ) {
		// Read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(cinfo, &rowPtr[rowsRead], cinfo->output_height - rowsRead);
	}

	// Delete the temporary row pointers
	delete [] rowPtr;

	// Finish decompressing the data
	jpeg_finish_decompress(cinfo);
}


}
