#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "graphic.h"

#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240

typedef struct {
    int x, y;
} l_map;

typedef struct {
    int keycode;
    int add_x;
    int add_y;
    int counter;
    int stop;
    int down;
} scroll_t;

enum {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
};

SDL_Window *window;
SDL_Renderer *render;
SDL_Event event;
anim_file_t *a;
layer_t l;
l_map *map = NULL;
int quit_flag = 0;
int dx = 0, dy = 0;
int game_keycode = KEY_NONE, game_move = 0, game_down = 0, game_lock = 0;
scroll_t scr = { KEY_NONE, +1, +1, 0, 1, 0 };

void send_error( int code )
{
    printf( "%s", SDL_GetError() );
    exit( code );
}

void game_init( void )
{
    int i;

    srand( time( NULL ) );
    window = SDL_CreateWindow( "Pixel Story", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( window == NULL ) {
        send_error( EXIT_FAILURE );
    }
    render = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | 
        SDL_RENDERER_PRESENTVSYNC );
    if ( render == NULL ) {
        send_error( EXIT_FAILURE );
    }
    ani_init( render, "../config/img_monitor.txt", &a );
    map = (l_map *) calloc( a->anim_count, sizeof(l_map) );
    for ( i = 0; i < a->anim_count; i++ ) {
        map[i].x = ( ( rand() % 180 / 10 ) + 1 ) * 16;
        map[i].y = ( ( rand() % 130 / 10 )+ 1 ) * 16;
        printf( "%d = [%d, %d]\n", i, map[i].x, map[i].y );
    }
    l.tex = IMG_LoadTexture( render, "../game_art/tileset.gif" );
    for ( i = 0; i < 100; i++ ) {
        l.map[rand()%20][rand()%15] = 21;
    }
}

void game_kscroll( int key )
{
    if ( scr.stop && !scr.counter ) {
        scr.keycode = key;
        scr.add_x = +1;
        scr.add_y = +1;
        scr.counter = 16;
        scr.stop = 0;
    }
}

void game_scroll( void )
{
    static int counter = 0;

    if ( scr.counter && !scr.stop ) {
        switch ( scr.keycode ) {
            case KEY_UP:
                dy += scr.add_y;
                break;
            case KEY_DOWN:
                dy -= scr.add_y;
                break;
            case KEY_LEFT:
                dx += scr.add_x;
                break;
            case KEY_RIGHT:
                dx -= scr.add_x;
                break;
        }
        scr.counter--;
        counter++;
    } else if ( !scr.counter && !scr.stop && !scr.down ) {
        scr.counter = 16;
        switch ( scr.keycode ) {
            case KEY_UP:
            case KEY_DOWN:
                scr.add_y = -scr.add_y;
                break;
            case KEY_LEFT:
            case KEY_RIGHT:
                scr.add_x = -scr.add_x;
                break;
        }
    }
    if ( counter > 31 ) {
        scr.keycode = KEY_NONE;
        scr.stop = 1;
        counter = 0;
    }
}

void game_event( SDL_Event *event )
{
    SDL_PollEvent( event );
    switch ( event->type ) {
        case SDL_QUIT:
            quit_flag = 1;
            break;
        case SDL_KEYDOWN:
            switch ( event->key.keysym.sym ) {
                case SDLK_w:
                    game_kscroll( KEY_UP );
                    break;
                case SDLK_s:
                    game_kscroll( KEY_DOWN );
                    break;
                case SDLK_a:
                    game_kscroll( KEY_LEFT );
                    break;
                case SDLK_d:
                    game_kscroll( KEY_RIGHT );
                    break;
            }
            scr.down = 1;
            break;
        case SDL_KEYUP:
            scr.down = 0;
            break;
    }
}

void game_loop( void )
{
    game_scroll();
}

void game_render( void )
{
    int i, j, xp, yp;
    
    xp = ( dx % 32 );
    yp = ( dy % 32 );
    SDL_RenderClear( render );
    for ( i = -1; i < 21; i++ ) {
        for ( j = -1; j < 16; j++ ) {
            tile_draw( render, l.tex, 55, i * 16 + ( xp / 2 ), j * 16 + ( yp / 2 ) );
        }
    }
    for ( i = 0; i < 20; i++ ) {
        for ( j = 0; j < 15; j++ ) {
            tile_draw( render, l.tex, l.map[i][j], i * 16 + ( dx * 2 ), j * 16 + ( dy * 2 ) );
        }
    }
    for ( i = 0; i < a->anim_count * a->frm_size; i += a->frm_size ) {
        ani_draw( render, a, i / a->frm_size, map[i/16].x + ( dx * 2 ), map[i/16].y + ( dy * 2 ) );
    }
    SDL_RenderPresent( render );
}

void game_destroy( void )
{
    free( map );
    ani_destroy( a );
    SDL_DestroyTexture( l.tex );
    SDL_DestroyRenderer( render );
    SDL_DestroyWindow( window );
    SDL_Quit();
}

int main( int argc, char *argv[] )
{
    game_init();
    while ( !quit_flag ) {
        game_event( &event );
        game_loop();
        game_render();
    }
    game_destroy();
    return EXIT_SUCCESS;
}