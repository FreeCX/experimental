#include "gif.h"

const char gif_info_01[] = 
    "gif.width      = %u\n"
    "gif.height     = %u\n"
    "gif.pack       = %u\n"
    "   .color_type = %u\n"
    "   .color      = %u\n"
    "   .sort_flag  = %u\n"
    "   .size       = %u\n"
    "gif.background = %u\n"
    "gif.reserved   = %u\n";

void gif_info( gif_fmt_t *h )
{
    int8 i;
    int16 color_type, color, sort_flag, size;

    printf( "gif.signature  = " );
    for ( i = 0; i < 6; i++ ) {
        printf( "%c", h->signature[i] );
    }
    printf( "\n" );
    color_type = getbits8( h->pack, 7, 1 );
    color = getbits8( h->pack, 6, 3 );
    sort_flag = getbits8( h->pack, 3, 1 );
    size = getbits8( h->pack, 2, 3 );
    printf( gif_info_01, h->width, h->height, h->pack, color_type, 
        color, sort_flag, size, h->background, h->reserved );
}

int8 gif_is_correct( gif_fmt_t *h )
{
    if ( h->signature[0] != 'G' && h->signature[1] != 'I' && 
         h->signature[2] != 'F' ) {
        return STATUS_IMG_INCORRECT;
    }
    return STATUS_SUCCESS;
}

int8 gif_load( FILE *f, image_t *img )
{
    gif_fmt_t h;

    memset( &h, 0, sizeof( gif_fmt_t ) );
    fread( &h, sizeof( gif_fmt_t ), 1, f );
    if ( gif_is_correct( &h ) == STATUS_IMG_INCORRECT ) {
        fseek( f, 0, SEEK_SET );
        return STATUS_IMG_INCORRECT;
    }
    if ( __DEBUG_FLAG__ ) {
        gif_info( &h );
    }
    return STATUS_SUCCESS;
}

int8 gif_save( FILE *f, image_t *img )
{
    // input code here
    return STATUS_SUCCESS;
}
