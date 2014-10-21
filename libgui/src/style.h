#ifndef __UI_STYLE_H__
#define __UI_STYLE_H__

#include "data.h"

struct style_button {
    uint32 background_color;
    uint32 left_color;
    uint32 right_color;
    uint32 top_color;
    uint32 bottom_color;
    uint32 focus_color;
    // add other styles: font ...
};
typedef struct style_button style_button_t;

struct ui_style {
    style_button_t button;
    // add other elements
};
typedef struct ui_style ui_style_t;

void ui_style_load( const char *filename );
ui_style_t *ui_style_get( void );

#endif
