#ifndef __FONT_H__
#define __FONT_H__

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define FONT_STEP           8

enum {
    A_NULL_OBJECT = 0,
    A_SUCCESS,
    A_ERROR_OPEN_FILE,
    A_ERROR_LOAD_TEXTURE,
};

struct font_table {
    int f_height;
    int f_width;
    int t_height;
    int t_width;
    int * table;
    SDL_Texture * font;
};
typedef struct font_table font_table_t;

#endif