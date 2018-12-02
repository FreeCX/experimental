#ifndef __LOADER_H__
#define __LOADER_H__

#include "bmp.h"
#include "error.h"
#include "gif.h"
#include "pcx.h"
#include "tga.h"

extern uint8_t __DEBUG_FLAG__;

void img_info(void);
void img_version(uint8_t *major, uint8_t *minor);
void img_debug(uint8_t param);
int8_t img_null(FILE *f, image_t *h);
uint8_t img_load(char *filename, image_t *img);
uint8_t img_close(image_t *img);

#endif
