#ifndef __UI_DRAW_H__
#define __UI_DRAW_H__

#include <string.h>
#include "data.h"
#include "error.h"

enum {
    UI_DRAW_NONE = 0,
    UI_DRAW_POINT,
    UI_DRAW_LINE,
    UI_DRAW_LINE_STRIP
};

enum {
    UI_RENDER_NONE = 0,
    UI_RENDER_OPENGL
};

#define DATA_SIZE 100

void ui_draw_init( uint8 render );
void ui_draw_begin( uint8 type );
void ui_draw_vertex2i( int32 x, int32 y );
void ui_draw_vertex2f( float x, float y );
void ui_draw_end( void );

uint8 ui_null_render( void );
uint8 ui_opengl_render( void );

#endif
