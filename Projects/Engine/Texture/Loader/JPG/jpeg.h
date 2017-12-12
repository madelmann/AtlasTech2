
#ifndef _Texture_Format_jpeg_h_
#define _Texture_Format_jpeg_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


struct tImage
{
	int channels;			// The channels in the image (3 = RGB : 4 = RGBA)
	int sizeX;				// The width of the image in pixels
	int sizeY;				// The height of the image in pixels
	unsigned char *data;	// The image pixel data
};


#endif
