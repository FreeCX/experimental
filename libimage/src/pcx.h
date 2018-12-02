#ifndef __PCX_H__
#define __PCX_H__

#include "data.h"
#include "error.h"

#pragma pack(push, 1)
struct pcx_fmt {
    uint8_t manufacturer;  // Constant Flag, 10 = ZSoft .pcx
    uint8_t version;       // Version information [ 0, 2, 3, 4, 5 ]
    uint8_t encoding;      // 1 = .PCX run length encoding
    uint8_t bpp;           // Number of bits to represent a pixel (per Plane) - 1, 2, 4, or 8
    uint16_t xmin;         //
    uint16_t ymin;         // Image
    uint16_t xmax;         // Dimensions
    uint16_t ymax;         //
    uint16_t hdpi;         // Horizontal Resolution of image in DPI*
    uint16_t vdpi;         // Vertical Resolution of image in DPI*
    uint8_t colormap[48];  // Color palette setting
    uint8_t reserved;      // Should be set to 0
    uint8_t nplanes;       // Number of color planes
    uint16_t bpl;          // Number of bytes to allocate for a scanline plane
    uint16_t pallete_info; // How to interpret palette - 1 = Color/BW, 2 = Grayscale
    uint16_t hscreen_size; // Horizontal screen size in pixels. New field found only in PB IV/IV Plus
    uint16_t vscreen_size; // Vertical screen size in pixels. New field found only in PB IV/IV Plus
    uint8_t filler[54];    // Blank to fill out 128 byte header
};
#pragma pack(pop)
typedef struct pcx_fmt pcx_fmt_t;

extern uint8_t __DEBUG_FLAG__;

void pcx_info(pcx_fmt_t *h);
int8_t pcx_is_correct(pcx_fmt_t *h);
int8_t pcx_load(FILE *f, image_t *img);
int8_t pcx_save(FILE *f, image_t *img);

#endif
