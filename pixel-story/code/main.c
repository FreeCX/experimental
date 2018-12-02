#include "graphic.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

typedef struct {
    int x, y;
} l_map;

enum { KEY_NONE = 0, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT };

typedef struct {
    SDL_Texture *tex;
    int x, y;
    int counter;
    int keycode;
} player_t;

SDL_Window *window;
SDL_Renderer *render;
SDL_Event event;
anim_file_t *a;
layer_t l;
l_map *map = NULL;
int quit_flag = 0;
player_t player = {NULL, 0, 0, 0, KEY_NONE};

void send_error(int code) {
    printf("%s", SDL_GetError());
    exit(code);
}

void game_init(void) {
    int i;

    srand(time(NULL));
    window = SDL_CreateWindow("Pixel Story", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        send_error(EXIT_FAILURE);
    }
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == NULL) {
        send_error(EXIT_FAILURE);
    }
    ani_init(render, "../config/img_monitor.txt", &a);
    map = (l_map *)calloc(a->anim_count, sizeof(l_map));
    for (i = 0; i < a->anim_count; i++) {
        map[i].x = ((rand() % 180 / 10) + 1) * 16;
        map[i].y = ((rand() % 130 / 10) + 1) * 16;
        printf("%d = [%d, %d]\n", i, map[i].x, map[i].y);
    }
    l.tex = IMG_LoadTexture(render, "../game_art/tileset.gif");
    for (i = 0; i < 100; i++) {
        l.map[rand() % 20][rand() % 15] = 21;
    }
    l.map[0][0] = 0;
    player.tex = IMG_LoadTexture(render, "../game_art/img_keyboard.gif");
}

void player_move(int keycode) {
    player.keycode = keycode;
    player.counter = 8;
}

void game_event(SDL_Event *event) {
    SDL_PollEvent(event);
    switch (event->type) {
        case SDL_QUIT:
            quit_flag = 1;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_w:
                    player_move(KEY_UP);
                    break;
                case SDLK_s:
                    player_move(KEY_DOWN);
                    break;
                case SDLK_a:
                    player_move(KEY_LEFT);
                    break;
                case SDLK_d:
                    player_move(KEY_RIGHT);
                    break;
            }
            break;
        case SDL_KEYUP:
            break;
    }
}

void game_loop(void) {
    int i, j;

    i = player.x / 16;
    j = player.y / 16;
    printf("[%d, %d]\n", i, j);
    if (player.counter) {
        switch (player.keycode) {
            case KEY_UP:
                if (l.map[i][j - 1] == 0 && j > 0) {
                    player.y--;
                }
                break;
            case KEY_DOWN:
                if (l.map[i][j + 1] == 0 && j < 14) {
                    player.y++;
                }
                break;
            case KEY_LEFT:
                if (l.map[i - 1][j] == 0 && i > 0) {
                    player.x--;
                }
                break;
            case KEY_RIGHT:
                if (l.map[i + 1][j] == 0 && i < 19) {
                    player.x++;
                }
                break;
        }
        player.counter--;
    }
}

void game_render(void) {
    int i, j;

    SDL_RenderClear(render);
    for (i = -1; i < 21; i++) {
        for (j = -1; j < 16; j++) {
            tile_draw(render, l.tex, 55, i * 16, j * 16);
        }
    }
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 15; j++) {
            tile_draw(render, l.tex, l.map[i][j], i * 16, j * 16);
        }
    }
    for (i = 0; i < a->anim_count * a->frm_size; i += a->frm_size) {
        ani_draw(render, a, i / a->frm_size, map[i / 16].x, map[i / 16].y);
    }
    tile_draw(render, player.tex, 1, player.x, player.y);
    SDL_RenderPresent(render);
}

void game_destroy(void) {
    free(map);
    ani_destroy(a);
    SDL_DestroyTexture(l.tex);
    SDL_DestroyTexture(player.tex);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    game_init();
    while (!quit_flag) {
        game_event(&event);
        game_loop();
        game_render();
    }
    game_destroy();
    return EXIT_SUCCESS;
}