#ifndef __LOADER_H__
#define __LOADER_H__

#include "bmp.h"
#include "pcx.h"
#include "tga.h"
#include "error.h"

extern uint8 __DEBUG_FLAG__;

enum {
	FORMAT_NONE,
	FORMAT_BMP,
	FORMAT_PCX,
	FORMAT_TGA
};

void img_debug( uint8 param );
uint8 img_load( char *filename, image_t *img );
uint8 img_close( image_t *img );

#endif
