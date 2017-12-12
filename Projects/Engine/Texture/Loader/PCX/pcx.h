
#ifndef _Texture_Loader_PCX_pcx_h_
#define _Texture_Loader_PCX_pcx_h_


// Library includes
#include <iostream>
#include <string>

// Project includes
#include <Driver/GLWrapper.h>

// Forward declarations

// Namespace declarations


struct gl_texture_t
{
	GLsizei	width;
	GLsizei	height;

	GLenum	format;
	GLint	internalFormat;
	GLuint	id;

	GLubyte	*texels;
};


#pragma pack(1)
/* PCX header */
struct pcx_header_t 
{
	GLubyte		manufacturer;
	GLubyte		version;
	GLubyte		encoding;
	GLubyte		bitsPerPixel;

	GLushort	xmin, ymin;
	GLushort	xmax, ymax;
	GLushort	horzRes, vertRes;

	GLubyte		palette[48];
	GLubyte		reserved;
	GLubyte		numColorPlanes;

	GLushort	bytesPerScanLine;
	GLushort	paletteType;
	GLushort	horzSize, vertSize;

	GLubyte		padding[54];
};
#pragma pack(4)


void ReadPCX1bit(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo);
void ReadPCX4bits(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo);
void ReadPCX8bits(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo);
void ReadPCX24bits(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo);


#endif
