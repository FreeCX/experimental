#ifndef __GIF_H__
#define __GIF_H__

#include "data.h"
#include "error.h"

struct gif_block {
    // Image Descriptor
    uint8_t img_separator; // Image separator = ','
    uint16_t img_left;     // Start of image in pixels from the left side of the screen
    uint16_t img_top;      // Start of image in pixels from the top of the screen
    uint16_t img_width;    // Width of the image in pixels
    uint16_t img_height;   // Height of the image in pixels
    uint8_t pack02;        // |M|I|0|0|0|pixel|
                           // M=0 - Use global color map, ignore 'pixel'
                           // M=1 - Local color map follows, use 'pixel'
                           // I=0/1 - Image formatted in Sequential/Interlaced order
                           // pixel+1 - # bits per pixel for this image
    // Local color map
    // Raster data
    uint8_t code_size;  //
    uint8_t byte_count; //
    uint8_t *data;      //
    uint8_t zero_byte;  //
};
typedef struct gif_block gif_block_t;

struct gif_fmt {
    uint8_t signature[6]; // GIFxxx
    // 7 6 5 4 3 2 1 0 (LSB first)
    uint16_t width;     // Raster width in pixels
    uint16_t height;    // Raster height in pixels
    uint8_t pack;       // |M|  cr |0|pixel|
                        // M = 1, Global color map follows Descriptor [1bit]
                        // cr+1 = # bits of color resolution [3bit]
                        // null [1bit = 0]
                        // pixel+1 = # bits/pixel in image [3bit]
    uint8_t background; // background = Color index of screen background
    uint8_t reserved;   // reserved [8bit = 0]

    uint8_t *glb_colormap;  // Red = 8bit, Green = 8bit, Blue = 8bit
                            // size = 2**(# bits per pixel)
                            // <map_value> = <component_value>*255/(2**<nbits>-1)
    gif_block_t *block;     // repeat n times
    uint8_t gif_terminator; // Termination of gif file [ 0x3B or ';' ]
};
typedef struct gif_fmt gif_fmt_t;

extern uint8_t __DEBUG_FLAG__;

void gif_info(gif_fmt_t *h);
int8_t gif_is_correct(gif_fmt_t *h);
int8_t gif_load(FILE *f, image_t *img);
int8_t gif_save(FILE *f, image_t *img);

#endif
