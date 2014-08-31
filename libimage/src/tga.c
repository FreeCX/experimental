#include "tga.h"

const char tga_info_01[] = 
    "tga.id_length   = %u\n"
    "tga.color_map   = %u\n"
    "tga.img_type    = %u\n"
    "tga.entry_index = %u\n"
    "tga.length      = %u\n"
    "tga.entry_size  = %u\n"
    "tga.x_origin    = %u\n"
    "tga.y_origin    = %u\n"
    "tga.img_width   = %u\n"
    "tga.img_height  = %u\n"
    "tga.img_depth   = %u\n"
    "tga.img_descr   = %u\n";

void tga_info( tga_fmt_t *h )
{
    printf( tga_info_01, h->id_length, h->color_map, h->img_type, h->entry_index, h->length, h->entry_size, 
            h->x_origin, h->y_origin, h->img_width, h->img_height, h->img_depth, h->img_descriptor );
}

int8 tga_is_correct( tga_fmt_t *h )
{
    if ( h->img_depth != 8 && h->img_depth != 24 && h->img_depth != 32 ) {
        return STATUS_IMG_INCORRECT;
    }
    return STATUS_SUCCESS;
}

int8 tga_load( FILE *f, image_t *img )
{
    uint32 xsize, ysize, image_size;
    uint8 *data, depth;
    size_t i, j;
    tga_fmt_t h;

    memset( &h, 0, sizeof( tga_fmt_t ) );
    fread( &h, sizeof( tga_fmt_t ), 1, f );
    if ( tga_is_correct( &h ) == STATUS_IMG_INCORRECT ) {
        fseek( f, 0, SEEK_SET );
        return STATUS_IMG_INCORRECT;
    }
    if ( __DEBUG_FLAG__ ) {
        tga_info( &h );
    }
    xsize = h.img_width;
    ysize = h.img_height;
    depth = h.img_depth / 8;
    image_size = xsize * ysize * depth;
    switch ( h.img_type ) {
        case 0: // none
        case 1: // indexed
        case 2: // rgb
        case 3: // grey
            data = (uint8 *) malloc( image_size * sizeof(uint8) );
            fread( data, image_size, 1, f );
            img->data = data;
            img->bpp = depth;
            img->width = xsize;
            img->height = ysize;
            break;
        // case +8: // rle packed
        //  break;
        default:
            img_module_error( "tga image type %u not supported!", h.img_type );
            break;
    }
    if ( h.y_origin == ysize ) {
        uint16 line_c = 0;
        i = j = 0;
        do { 
            swap8( &data[i], &data[ xsize * depth * ( ysize - line_c - 1 ) + j] );
            if ( j > depth * ( xsize - 1 ) + 1 ) {
                j = 0;
                line_c++;
            } else {
                j++;
            }
        } while ( ++i < xsize * ysize * depth / 2 );
    }
    // if ( h.x_origin == xsize ) {
    //  uint16 line_c = 0;
    //  i = j = 0;
    //  do {
    //      if ( i % ( xsize * depth / 2 ) == 0 ) {
    //          i += xsize * depth / 2;
    //          line_c++;
    //      }
    //      swap8( &data[i], &data[ xsize * depth * (line_c+1) - i] );
    //  } while ( ++i < xsize * ysize * depth / 2 );
    // }
    switch ( depth ) {
        case 1: // mono image
            img->c_format = GL_LUMINANCE8;
            break;
        case 3: // bgr image
            img->c_format = IMG_BGR;
            break;
        case 4: // rgba image
            img->c_format = IMG_BGRA;
            break;
    }
    return STATUS_SUCCESS;
}

int8 tga_save( FILE *f, image_t *img )
{
    // input code here
    return STATUS_SUCCESS;
}