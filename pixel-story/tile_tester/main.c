#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../code/graphic.h"

#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT 		240
#define TILE_STEP           16
#define FULL_WIDTH          SCREEN_WIDTH  + 2 * TILE_STEP
#define FULL_HEIGHT         SCREEN_HEIGHT + 2 * TILE_STEP

SDL_Window *window;
SDL_Renderer *render;
SDL_Rect pos = { 0, 0, 16, 16 };
SDL_Texture *tex = NULL;
anim_file_t *a;
char no_texture[64] = {
	0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 
	0x10, 0x00, 0x80, 0x00, 0x00, 0xFF, 0x00, 0xFF, 
	0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x1F, 0x84, 
	0x6F, 0xA1, 0xAB, 0x88, 0xCC, 0xDC, 0x81, 0x4B, 
	0x26, 0x1A, 0x2C, 0xC0, 0xD9, 0x72, 0xFD, 0x79, 
	0x1C, 0x06, 0x92, 0x22, 0x55, 0xA2, 0x27, 0x94, 
	0xB2, 0x6B, 0xF4, 0x56, 0x56, 0x01, 0x00, 0x3B
};

int x_step = 0;
int y_step = 0;

void send_error( int code )
{
	printf( "%s\n", SDL_GetError() );
	exit( code );
}

int main( int argc, char *argv[] )
{
	SDL_Event event;
	int quit_flag = 0, i, j, dx = 0, dy = 0, anim = 0;
	int k_x = 0, k_y = 0;

	window = SDL_CreateWindow( "Tile Tester", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if ( window == NULL ) {
		send_error( EXIT_FAILURE );
	}
	render = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | 
		SDL_RENDERER_PRESENTVSYNC );
	if ( render == NULL ) {
		send_error( EXIT_FAILURE );
	}
	if ( argc > 1 ) {
		tex = IMG_LoadTexture( render, argv[1] );
		if ( argc > 2 && 
			ani_init( render, argv[2], &a ) == A_SUCCESS ) {
			anim = 1;
		} else {
			printf( "%s\n", SDL_GetError() );
		}
	} else {
		printf( "usage: %s <background> <config file>\n", argv[0] );
		tex = NULL;
	}
	if ( tex == NULL ) {
		SDL_RWops *rw = NULL;
		printf( "%s\n", SDL_GetError() );
		rw = SDL_RWFromMem( no_texture, sizeof(no_texture) );
		tex = IMG_LoadTexture_RW( render, rw, 1 );
	}
	while ( !quit_flag ) {
		SDL_PollEvent( &event );
		switch ( event.type ) {
			case SDL_QUIT:
				quit_flag = 1;
				break;
        	case SDL_KEYDOWN:
	            switch ( event.key.keysym.sym ) {
	                case SDLK_w:
	                	y_step = -1;
	                    break;
	                case SDLK_s:
	                	y_step = +1;
	                    break;
	                case SDLK_a:
	                    x_step = -1;
	                    break;
	                case SDLK_d:
	                    x_step = +1;
	                    break;
	                case SDLK_SPACE:
	                	k_x = k_y = x_step = y_step = 0;
	                	break;
	            }
            break;
		}
		SDL_RenderClear( render );
		for ( i = 0; i < FULL_WIDTH; i += TILE_STEP ) {
			for ( j = 0; j < FULL_HEIGHT; j += TILE_STEP ) {
				pos.x = i+k_x-TILE_STEP; pos.y = j+k_y-TILE_STEP;
				SDL_RenderCopy( render, tex, 0, &pos );
			}
		}
		if ( anim ) {
			for ( i = 0; i < a->anim_count * TILE_STEP; i += TILE_STEP ) {
				if ( dx > SCREEN_WIDTH ) {
					dx = 0; dy += TILE_STEP;
				}
				ani_draw( render, a, i / TILE_STEP, dx, dy );
				dx += TILE_STEP ;
			}
			dx = dy = 0;
		}
		SDL_RenderPresent( render );
		if ( abs( k_x ) < TILE_STEP ) {
			k_x += x_step;
		} else {
			k_x = 0;
		}
		if ( abs( k_y ) < TILE_STEP ) {
			k_y += y_step;
		} else {
			k_y = 0;
		}
	}
	ani_destroy( a );
	SDL_DestroyTexture( tex );
	SDL_DestroyRenderer( render );
	SDL_DestroyWindow( window );
	SDL_Quit();
	return EXIT_SUCCESS;
}