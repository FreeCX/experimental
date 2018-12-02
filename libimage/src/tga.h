#ifndef __TGA_H__
#define __TGA_H__

#include "data.h"
#include "error.h"

#pragma pack(push, 1)
struct tga_fmt {
    uint8_t id_length;      // size of ID field (0 usually)
    uint8_t color_map;      // type of color map:
                            // 0 = none, 1 = has palette
    uint8_t img_type;       // type of image: 0 - none, 1 - indexed
                            // 2 = rgb, 3 - grey, +8 = rle packed
    uint16_t entry_index;   // first colour map entry in pallete
    uint16_t length;        // number of colours in palette
    uint8_t entry_size;     // number of bits per palette entry:
                            // 15, 16, 24, 32
    uint16_t x_origin;      // image x origin
    uint16_t y_origin;      // image y origin
    uint16_t img_width;     // image width in pixels
    uint16_t img_height;    // image height in pixels
    uint8_t img_depth;      // image bits per pixel 8, 16, 24, 32
    uint8_t img_descriptor; // image descriptor bits (vh flip bits)
};
#pragma pack(pop)
typedef struct tga_fmt tga_fmt_t;

extern uint8_t __DEBUG_FLAG__;

void tga_info(tga_fmt_t *h);
int8_t tga_is_correct(tga_fmt_t *h);
int8_t tga_load(FILE *f, image_t *img);
int8_t tga_save(FILE *f, image_t *img);

#endif
