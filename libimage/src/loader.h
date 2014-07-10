#ifndef __LOADER_H__
#define __LOADER_H__

#include "bmp.h"
#include "pcx.h"
#include "tga.h"
#include "gif.h"
#include "error.h"

extern uint8 __DEBUG_FLAG__;

void img_debug( uint8 param );
int8 img_null( FILE *f, image_t *h );
uint8 img_load( char *filename, image_t *img );
uint8 img_close( image_t *img );

#endif
