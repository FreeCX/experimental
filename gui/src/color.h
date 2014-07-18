#ifndef __UI_COLOR_H__
#define __UI_COLOR_H__

#include <math.h>

void rgb_to_hsv( unsigned int color, double hsv[3] );
void hsv_to_rgb( unsigned int *color, double hsv[3] );

#endif