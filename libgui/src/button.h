#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include <assert.h>
#include <stdlib.h>
#include "data.h"
#include "draw.h"
#include "string.h"
#include "style.h"

typedef void ( *ui_button_callback )();

#pragma pack( push, 1 )
struct ui_button_flag {
    uint8 draw     : 1;
    uint8 click    : 1;
    uint8 focus    : 1;
    uint8 enable   : 1;
    uint8 reserved : 4;
};
#pragma pack( pop )
typedef ui_button_flag_t;

#pragma pack( push, 1 )
struct ui_button {
    uint32 x_pos, y_pos;
    uint32 width, height;
    uint16 id;
    char *label;
    ui_button_flag_t flag;
    ui_button_callback func;
    struct ui_button *next;
};
#pragma pack( pop )
typedef struct ui_button ui_button_t;

uint16 ui_button_create( uint32 x, uint32 y, uint32 w, uint32 h, const char *label, ui_button_callback func );
void ui_button_flag( uint16 id, ui_button_flag_t flag );
void ui_button_label( uint16 id, const char *label );
void ui_button_draw( void );
void ui_button_destroy( void );

#endif