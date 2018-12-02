#ifndef __BMP_H__
#define __BMP_H__

#include "data.h"
#include "error.h"

#pragma pack(push, 1)
struct bmp_fmt {
    uint16_t magic;        // magic number 'BM'
    uint32_t file_size;    // size of bmp file
    uint32_t reserved;     // application specific
    uint32_t img_offset;   // offset where the pixel array can be found
    uint32_t dib_size;     // number of bytes in the DIB header
    uint32_t width;        // image width in pixels
    uint32_t height;       // image height in pixels
    uint16_t planes;       // number of color planes being used
    uint16_t bpp;          // image bits per pixel
    uint32_t compression;  // pixel compression
    uint32_t data_size;    // size of the raw data in the pixel array
    uint32_t h_resolution; // horizontal resolution of the image
    uint32_t v_resolution; // vertical resolution of the image
    uint32_t palette;      // number of colors in the palette
    uint32_t colors;       // important colors
};
#pragma pack(pop)
typedef struct bmp_fmt bmp_fmt_t;

#define BMP_MAGIC 0x424D

extern uint8_t __DEBUG_FLAG__;

void bmp_info(bmp_fmt_t *h);
int8_t bmp_is_correct(bmp_fmt_t *h);
int8_t bmp_load(FILE *f, image_t *img);
int8_t bmp_save(FILE *f, image_t *img);

#endif
