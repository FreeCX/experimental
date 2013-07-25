#ifndef __TGA_H__
#define __TGA_H__

#include "data.h"

struct cmap_spec {
	uint16 	entry_index;            // first colour map entry in pallete
	uint16 	length;                 // number of colours in palette
	uint8 	entry_size;             // number of bits per palette entry:
                                    // 15, 16, 24, 32
};
typedef struct cmap_spec cmap_spec_t;

struct img_spec {
	uint16 	x_origin;               // image x origin
	uint16 	y_origin;               // image y origin
	uint16 	img_width;              // image width in pixels
	uint16 	img_height;             // image height in pixels
	uint8 	img_depth;              // image bits per pixel 8, 16, 24, 32
	uint8 	img_descriptor;         // image descriptor bits (vh flip bits)
};
typedef struct img_spec img_spec_t;

struct tga_fmt {
	uint8 	id_length;              // size of ID field (0 usually)
	uint8 	color_map;              // type of color map: 
                                    // 0 = none, 1 = has palette
	uint8 	img_type;               // type of image: 0 - none, 1 - indexed
                                    // 2 = rgb, 3 - grey, +8 = rle packed
	cmap_spec_t cspec;
	img_spec_t 	ispec;
};
typedef struct tga_fmt tga_fmt_t;

extern uint8 __DEBUG_FLAG__;

void tga_info( tga_fmt_t *h );
int8 tga_load( FILE *f, image_t *img );
int8 tga_save( FILE *f, image_t *img );

#endif
