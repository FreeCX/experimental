#include "draw.h"

static uint8 element_type = 0;
static uint8 current = 0;
static float *data = NULL;

void ui_draw_begin( uint8 type )
{
    element_type = type;
    if ( data == NULL ) {
        data = (float *) malloc( DATA_SIZE * sizeof( float ) );
    }
}

void ui_draw_vertex2i( int32 x, int32 y )
{
    ui_draw_vertex2f( (float) x, (float) y );
}

void ui_draw_vertex2f( float x, float y )
{
    if ( current < DATA_SIZE - 1 ) {
        data[current++] = x;
        data[current++] = y;
    }
}

void ui_draw_end( void )
{
    if ( data != NULL ) {
        // input draw code: OpenGL, SDL & etc
        current = 0;
    }
}