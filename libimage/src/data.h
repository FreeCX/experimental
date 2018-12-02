#ifndef __DATA_H__
#define __DATA_H__

#include <GL/gl.h>
#include <GL/glext.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { IMG_RGB = GL_RGB, IMG_BGR = GL_BGR, IMG_RGBA = GL_RGBA, IMG_BGRA = GL_BGRA };

enum {
    STATUS_FAILED = 0,
    STATUS_SUCCESS,
    STATUS_IMG_INCORRECT,
};

struct image {
    uint32_t c_format;
    uint8_t *data;
    uint8_t bpp;
    uint32_t width, height;
};
typedef struct image image_t;

uint16_t bswap16(uint16_t input);
uint32_t bswap32(uint32_t input);
uint64_t bswap64(uint64_t input);
void swap8(uint8_t *a, uint8_t *b);
void swap16(uint16_t *a, uint16_t *b);
void swap32(uint32_t *a, uint32_t *b);
size_t fsize(FILE *f);
uint8_t getbits8(uint8_t a, uint8_t p, uint8_t n);
uint32_t dec2bin(uint8_t a);

#endif
