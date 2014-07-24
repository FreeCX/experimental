#include "draw.h"

static uint8 select_render = UI_RENDER_NONE;
static uint8 element_type = UI_DRAW_NONE;
static uint8 current = 0;
static float *data = NULL;

uint8 ( *renders[] )( void ) = {
    ui_null_render,
    ui_opengl_render
};
size_t fmt_size = sizeof( renders ) / sizeof( renders[0] );

void ui_draw_init( uint8 render )
{
    select_render = render;
}

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
    } else {
        ui_error_send( "stack overflow");
    }
}

void ui_draw_end( void )
{
    if ( data != NULL || current != 0 || element_type != UI_DRAW_NONE ) {
        if ( select_render <= fmt_size ) {
            renders[select_render](); 
        } else {
            ui_error_send( "unknown render" );
        }
        current = 0;
    }
}

uint8 ui_null_render( void )
{
    ui_error_send( "render is not set" );
}

uint8 ui_opengl_render( void )
{
    // input opengl render code
}