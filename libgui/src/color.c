#include "color.h"

double set_color[3] = { 0 };

void ui_color_rgb2hsv( const uint32 color, double hsv[] )
{
    double rd = (double) ( color >> 16 ) / 255.0;
    double gd = (double) ( ( color >> 8 ) & 0xFF ) / 255.0;
    double bd = (double) ( color & 0xFF ) / 255.0;
    double max = fmax( rd, fmax( gd, bd ) );
    double min = fmin( rd, fmin( gd, bd ) );
    double h = 0.0, s, v = max;
    double d = max - min;

    s = max == 0 ? 0 : d / max;
    if ( max == min ) {
        h = 0.0;
    } else {
        if ( max == rd ) {
            h = ( gd - bd ) / d + ( gd < bd ? 6.0 : 0.0 );
        } else if ( max == gd ) {
            h = ( bd - rd ) / d + 2.0;
        } else if ( max == bd ) {
            h = ( rd - gd ) / d + 4.0;
        }
        h /= 6.0;
    }
    hsv[0] = h;
    hsv[1] = s;
    hsv[2] = v;
}

void ui_color_hsv2rgb( uint32 * color, double hsv[] )
{
    double r, g, b;
    int i = (int)( hsv[0] * 6.0 );
    double f = hsv[0] * 6.0 - i;
    double p = hsv[2] * ( 1.0 - hsv[1] );
    double q = hsv[2] * ( 1.0 - f * hsv[1] );
    double t = hsv[2] * ( 1.0 - ( 1.0 - f ) * hsv[1] );

    switch ( i % 6 ) {
        case 0: r = hsv[2]; g = t; b = p; break;
        case 1: r = q; g = hsv[2]; b = p; break;
        case 2: r = p; g = hsv[2]; b = t; break;
        case 3: r = p; g = q; b = hsv[2]; break;
        case 4: r = t; g = p; b = hsv[2]; break;
        case 5: r = hsv[2]; g = p; b = q; break;
    }
    *color = ( (int)( r * 255 ) << 16 ) + ( (int)( g * 255 ) << 8 ) + (int)( b * 255 );
}

uint32 ui_color_sub( const uint32 a, const uint32 b )
{
    unsigned int result;
    double c[3], d[3], f[3];
    size_t i;

    rgb_to_hsv( a, c );
    rgb_to_hsv( b, d );
    for ( i = 0; i < 3; i++ ) {
        f[i] = fabs( c[i] + d[i] ) / 2.0;
    }
    hsv_to_rgb( &result, f );
    return result;
}

void ui_color_setu( const uint32 color )
{
    set_color[0] = (double) ( color >> 16 ) / 255.0;
    set_color[1] = (double) ( ( color >> 8 ) & 0xFF ) / 255.0;
    set_color[2] = (double) ( color & 0xFF ) / 255.0;
}

void ui_color_setv( double color[] )
{
    memcpy( set_color, color, 3 * sizeof( double ) );
}

void ui_color_set3u( const uint8 red, const uint8 green, const uint8 blue )
{
    set_color[0] = (double) red / 255.0;
    set_color[1] = (double) green / 255.0;
    set_color[2] = (double) blue / 255.0;
}

void ui_color_set3f( const float red, const float green, const float blue )
{
    set_color[0] = (double) red;
    set_color[1] = (double) green;
    set_color[2] = (double) blue;
}
