#ifndef __BMP_H__
#define __BMP_H__

#include "data.h"

#pragma pack( push, 1 )
struct bmp_fmt {
    uint16 magic;               // magic number 'BM'
    uint32 file_size;           // size of bmp file
    uint32 reserved;            // application specific
    uint32 img_offset;          // offset where the pixel array can be found
    uint32 dib_size;            // number of bytes in the DIB header
    uint32 width;               // image width in pixels
    uint32 height;              // image height in pixels
    uint16 planes;              // number of color planes being used
    uint16 bpp;                 // image bits per pixel
    uint32 compression;         // pixel compression
    uint32 data_size;           // size of the raw data in the pixel array
    uint32 h_resolution;        // horizontal resolution of the image
    uint32 v_resolution;        // vertical resolution of the image
    uint32 palette;             // number of colors in the palette
    uint32 colors;              // important colors
};
#pragma pack( pop )
typedef struct bmp_fmt bmp_fmt_t;

#define BMP_MAGIC 0x424D

extern uint8 __DEBUG_FLAG__;

void bmp_info( bmp_fmt_t *h );
int8 bmp_is_correct( bmp_fmt_t *h );
int8 bmp_load( FILE *f, image_t *img );
int8 bmp_save( FILE *f, image_t *img );

#endif
