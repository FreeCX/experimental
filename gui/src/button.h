#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "data.h"

typedef void ( *ui_button_callback )();

struct ui_button_flag {
    uint8 draw     : 1;
    uint8 click    : 1;
    uint8 focus    : 1;
    uint8 enable   : 1;
    uint8 reserved : 4;
};
typedef ui_button_flag_t;

struct ui_button {
    uint32 x_pos, y_pos;
    uint32 width, height;
    uint16 id;
    ui_button_flag_t flag;
    char *label;
    struct ui_button *next;
};
typedef struct ui_button ui_button_t;

#endif