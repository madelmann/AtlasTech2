
#ifndef _Texture_Loader_TGA_tga_h_
#define _Texture_Loader_TGA_tga_h_


// Library includes
#include <iostream>
#include <fstream>
#include <memory.h>

// Project includes
#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT  0x8
#define IMG_ERR_UNSUPPORTED 0x40

// Forward declarations

// Namespace declarations


namespace TextureSpace {


class TGAImg
{
public:
	TGAImg();
	~TGAImg();

	int load(const char* szFilename);
	int getBPP();
	int getWidth();
	int getHeight();
	unsigned char* getImg();       // Return a pointer to image data
	unsigned char* getPalette();   // Return a pointer to VGA palette

private:
	// Internal workers
	int ReadHeader();
	int LoadRawData();
	int LoadTgaRLEData();
	int LoadTgaPalette();
	void BGRtoRGB();
	void FlipImg();

	short int mWidth, mHeight, mBPP;
	unsigned long lImageSize;
	char bEnc;
	unsigned char *pImage, *pPalette, *pData;
};


}


#endif