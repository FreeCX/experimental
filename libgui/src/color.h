#ifndef __UI_COLOR_H__
#define __UI_COLOR_H__

#include <math.h>
#include "data.h"

void ui_color_rgb2hsv( const uint32 color, double hsv[3] );
void ui_color_hsv2rgb( uint32 *color, double hsv[3] );
uint32 ui_color_sub( const uint32 a, const uint32 b );
void ui_color_setu( const uint32 color );
void ui_color_setv( double color[3] );
void ui_color_set3u( const uint8 red, const uint8 green, const uint8 blue );
void ui_color_set3f( const float red, const float green, const float blue );

#endif