
// Header
#include "pcx.h"

// Library includes

// Project includes

// Namespace declarations


void ReadPCX1bit(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo)
{
	int y, i, bytes;
	int colorIndex;
	int rle_count = 0, rle_value = 0;
	GLubyte *ptr = texinfo->texels;

	for ( y = 0; y < texinfo->height; ++y ) {
		ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];
		bytes = hdr->bytesPerScanLine;

		/* Decode line number y */
		while ( bytes-- ) {
			if ( rle_count == 0 ) {
				if ( (rle_value = fgetc (fp)) < 0xc0 ) {
					rle_count = 1;
				}
				else {
					rle_count = rle_value - 0xc0;
					rle_value = fgetc (fp);
				}
			}

			rle_count--;

			for ( i = 7; i >= 0; --i, ptr += 3 ) {
				colorIndex = ((rle_value & (1 << i)) > 0);

				ptr[0] = hdr->palette[colorIndex * 3 + 0];
				ptr[1] = hdr->palette[colorIndex * 3 + 1];
				ptr[2] = hdr->palette[colorIndex * 3 + 2];
			}
		}
	}
}

void ReadPCX4bits(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo)
{
	GLubyte *colorIndex, *line;
	GLubyte *pLine, *ptr;
	int rle_count = 0, rle_value = 0;
	int x, y, c;
	int bytes;

	colorIndex = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width);
	line = (GLubyte *)malloc (sizeof (GLubyte) * hdr->bytesPerScanLine);

	for ( y = 0; y < texinfo->height; ++y ) {
		ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];

		memset(colorIndex, 0, texinfo->width * sizeof(GLubyte));

		for ( c = 0; c < 4; ++c ) {
			pLine = line;
			bytes = hdr->bytesPerScanLine;

			/* Decode line number y */
			while ( bytes-- ) {
				if ( rle_count == 0 ) {
					if ( (rle_value = fgetc (fp)) < 0xc0 ) {
						rle_count = 1;
					}
					else {
						rle_count = rle_value - 0xc0;
						rle_value = fgetc (fp);
					}
				}

				rle_count--;
				*(pLine++) = static_cast<GLubyte>(rle_value);
			}

			/* Compute line's color indexes */
			for ( x = 0; x < texinfo->width; ++x ) {
				if ( line[x / 8] & (128 >> (x % 8)) ) {
					colorIndex[x] += (1 << c);
				}
			}
		}

		/* Decode scan line.  color index => rgb  */
		for ( x = 0; x < texinfo->width; ++x, ptr += 3 ) {
			ptr[0] = hdr->palette[colorIndex[x] * 3 + 0];
			ptr[1] = hdr->palette[colorIndex[x] * 3 + 1];
			ptr[2] = hdr->palette[colorIndex[x] * 3 + 2];
		}
	}

	/* Release memory */
	free (colorIndex);
	free (line);
}

void ReadPCX8bits(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo)
{
	int rle_count = 0, rle_value = 0;
	GLubyte palette[768];
	GLubyte magic;
	GLubyte *ptr;
	fpos_t curpos;
	int y, bytes;

	/* The palette is contained in the last 769 bytes of the file */
	fgetpos (fp, &curpos);
	fseek (fp, -769, SEEK_END);
	magic = static_cast<GLubyte>(fgetc(fp));

	/* First byte must be equal to 0x0c (12) */
	if (magic != 0x0c) {
		fprintf (stderr, "error: colormap's first byte must be 0x0c! (%#x)\n", magic);

		free (texinfo->texels);
		texinfo->texels = NULL;
		return;
	}

	/* Read palette */
	fread (palette, sizeof (GLubyte), 768, fp);
	fsetpos (fp, &curpos);

	/* Read pixel data */
	for ( y = 0; y < texinfo->height; ++y ) {
		ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];
		bytes = hdr->bytesPerScanLine;

		/* Decode line number y */
		while ( bytes-- ) {
			if ( rle_count == 0 ) {
				if ( (rle_value = fgetc (fp)) < 0xc0 ) {
					rle_count = 1;
				}
				else {
					rle_count = rle_value - 0xc0;
					rle_value = fgetc (fp);
				}
			}

			rle_count--;

			ptr[0] = palette[rle_value * 3 + 0];
			ptr[1] = palette[rle_value * 3 + 1];
			ptr[2] = palette[rle_value * 3 + 2];
			ptr += 3;
		}
	}
}

void ReadPCX24bits(FILE *fp, const struct pcx_header_t *hdr, struct gl_texture_t *texinfo)
{
	GLubyte *ptr = texinfo->texels;
	int rle_count = 0, rle_value = 0;
	int y, c;
	int bytes;

	for ( y = 0; y < texinfo->height; ++y ) {
		/* For each color plane */
		for ( c = 0; c < 3; ++c ) {
			ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];
			bytes = hdr->bytesPerScanLine;

			/* Decode line number y */
			while ( bytes-- ) {
				if ( rle_count == 0 ) {
					if ( (rle_value = fgetc (fp)) < 0xc0 ) {
						rle_count = 1;
					}
					else {
						rle_count = rle_value - 0xc0;
						rle_value = fgetc (fp);
					}
				}

				rle_count--;
				ptr[c] = (GLubyte)rle_value;
				ptr += 3;
			}
		}
	}
}
