#include "button.h"

static uint16 global_id = 0;
ui_button_t *button_list = NULL;
ui_button_flag_t def = { 1, 0, 0, 1, 0 };

uint16 ui_button_create( uint32 x, uint32 y, uint32 w, uint32 h, const char *label, ui_button_callback func )
{
    size_t label_length = string_length( label );
    ui_button_t *p = ( ui_button_t * ) calloc( sizeof( ui_button_t ) );
    assert( p != NULL );
    p->x_pos = x; p->y_pos = y;
    p->width = w; p->height = h;
    p->flag = def;
    p->func = func;
    if ( label_length > 0 ) {
        p->label = (char *) malloc( label_length );
        if ( p->label != NULL ) {
            string_copy( label, p->label );
        }
    }
    p->next = button_list;
    button_list = p;
    return p->id = ++global_id;
}

void ui_button_flag( uint16 id, ui_button_flag_t flag )
{
    ui_button_t *p = button_list;

    while ( p->id != id ) {
        if ( p->next == NULL ) {
            return;
        }
        p = p->next;
    }
    p->flag = flag;
}

void ui_button_label( uint16 id, const char *label )
{
    ui_button_t *p = button_list;
    size_t label_length = string_length( label );

    while ( p->id != id ) {
        if ( p->next == NULL ) {
            return;
        }
        p = p->next;
    }
    if ( p->label != NULL ) {
        free( p->label );
    }
    if ( label_length > 0 ) {
        p->label = (char *) malloc( label_length );
        if ( p->label != NULL ) {
            string_copy( label, p->label );
        }
    }
}

void ui_button_draw( void )
{
    ui_button_t *p = button_list

    while ( p != NULL ) {
        /* testing draw */
        ui_color_setu( 0x515642 );
        ui_draw_begin( UI_DRAW_LINE_STRIP );
        ui_draw_vertex2i( p->x_pos, p->y_pos );
        ui_draw_vertex2i( p->x_pos + p->width, p->y_pos );
        ui_draw_vertex2i( p->x_pos + p->width, p->y_pos + p->height );
        ui_draw_vertex2i( p->x_pos, p->y_pos + p->height );
        ui_draw_end();
        /* testing draw */
        p = p->next;
    }
}

void ui_button_destroy( void )
{
    ui_button_t *p = button_list, *n;

    while ( p != NULL ) {
        n = c->next;
        if ( p->label != NULL ) {
            free( p->label );
        }
        free( c );
        c = n;
    }
}