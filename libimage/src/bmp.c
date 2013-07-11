#include "bmp.h"

char bmp_info_01[] = 
    "bmp.magic         = %x\n"
    "bmp.file_size     = %u\n"
    "bmp.reserved      = %u\n"
    "bmp.img_offset    = %u\n"
    "bmp.dib_size      = %u\n"
    "bmp.width         = %u\n"
    "bmp.height        = %u\n"
    "bmp.planes        = %u\n"
    "bmp.bpp           = %u\n"
    "bmp.compression   = %u\n"
    "bmp.data_size     = %u\n"
    "bmp.h_resolution  = %u\n"
    "bmp.v_resolution  = %u\n"
    "bmp.pallete       = %u\n"
    "bmp.colors        = %u\n";

void bmp_info( bmp_fmt_t *h )
{
    printf( bmp_info_01, h->magic, h->file_size, h->reserved, h->img_offset, 
        h->dib_size, h->width, h->height, h->planes, h->bpp, h->compression,
        h->data_size, h->h_resolution, h->v_resolution, h->palette, h->colors );
}

int8 bmp_load( FILE *f, image_t *img )
{
    bmp_fmt_t h;

    memset( &h, 0, sizeof(bmp_fmt_t) );
    fread( &h, 1, sizeof(bmp_fmt_t), f );
    bmp_info( &h );
	return STATUS_SUCCESS;
}

int8 bmp_save( FILE *f, image_t *img )
{
	return STATUS_SUCCESS;
}
