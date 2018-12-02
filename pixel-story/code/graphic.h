#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    A_NULL_OBJECT = 0,
    A_SUCCESS,
    A_ERROR_OPEN_FILE,
    A_ERROR_LOAD_TEXTURE,
};

struct anim {
    char *anim_name;
    int frm_count;
    int frm_select;
    int frm_rate;
    int counter;
    int dy;
    int id;
};
typedef struct anim anim_t;

struct anim_file {
    SDL_Texture *tex;
    int anim_count;
    int frm_size;
    anim_t *a;
};
typedef struct anim_file anim_file_t;

struct layer {
    SDL_Texture *tex;
    // 320x240 window [20x15 by 16x16 tile]
    unsigned short map[20][15];
    unsigned short id;
};
typedef struct layer layer_t;

int ani_init(SDL_Renderer *r, char *config, anim_file_t **a);
int ani_draw(SDL_Renderer *r, anim_file_t *obj, int id, int x, int y);
int ani_destroy(anim_file_t *obj);
void tile_draw(SDL_Renderer *r, SDL_Texture *t, short id, int x, int y);

#endif