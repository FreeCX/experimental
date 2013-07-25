#ifndef __PCX_H__
#define __PCX_H__

#include "data.h"

struct img_dim {
	uint16 	xmin;
	uint16 	ymin;
	uint16 	xmax;
	uint16 	ymax;
};
typedef struct img_dim img_dim_t;

struct pcx_fmt {
	uint8 	manufacturer;	// Constant Flag, 10 = ZSoft .pcx 
	uint8 	version;		// Version information [ 0, 2, 3, 4, 5 ]
	uint8 	encoding;		// 1 = .PCX run length encoding 
	uint8 	bpp;			// Number of bits to represent a pixel (per Plane) - 1, 2, 4, or 8 
	img_dim_t window;		// Image Dimensions [ see img_dim struct ]
	uint16	hdpi;			// Horizontal Resolution of image in DPI*
	uint16	vdpi;			// Vertical Resolution of image in DPI*
	uint8	colormap[48];	// Color palette setting
	uint8	reserved;		// Should be set to 0
	uint8	nplanes;		// Number of color planes
	uint16	bpl;			// Number of bytes to allocate for a scanline plane
	uint16	pallete_info;	// How to interpret palette - 1 = Color/BW, 2 = Grayscale
	uint16	hscreen_size;	// Horizontal screen size in pixels. New field found only in PB IV/IV Plus
	uint16	vscreen_size;	// Vertical screen size in pixels. New field found only in PB IV/IV Plus
	uint8	filler[54];		// Blank to fill out 128 byte header
};
typedef struct pcx_fmt pcx_fmt_t;

extern uint8 __DEBUG_FLAG__;

void pcx_info( pcx_fmt_t *h );
int8 pcx_load( FILE *f, image_t *img );
int8 pcx_save( FILE *f, image_t *img );

#endif