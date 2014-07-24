#ifndef __UI_DRAW_H__
#define __UI_DRAW_H__

#include "data.h"

enum {
    UI_DRAW_NONE = 0,
    UI_DRAW_POINT,
    UI_DRAW_LINE,
    UI_DRAW_LINE_STRIP
};

#define DATA_SIZE 100

void ui_draw_begin( uint8 type );
void ui_draw_vertex2i( int32 x, int32 y );
void ui_draw_vertex2f( float x, float y );
void ui_draw_end( void );

#endif