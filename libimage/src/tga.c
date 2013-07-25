#include "tga.h"

char tga_info_01[] = 
	"tga.id_length        = %u\n"
	"tga.color_map        = %u\n"
	"tga.img_type         = %u\n"
	"tga.cmap.entry_index = %u\n"
	"tga.cmap.length      = %u\n"
	"tga.cmap.entry_size  = %u\n"
	"tga.imgs.x_origin    = %u\n"
	"tga.imgs.y_origin    = %u\n"
	"tga.imgs.img_width   = %u\n"
	"tga.imgs.img_height  = %u\n"
	"tga.imgs.img_depth   = %u\n"
	"tga.imgs.img_descr   = %u\n";

static void buffer2tga( uint8 buffer[sizeof(tga_fmt_t)], tga_fmt_t *h )
{
	h->id_length = buffer[0];
	h->color_map = buffer[1];
    h->img_type = buffer[2];
    h->cspec.entry_index = buffer[3] + buffer[4] * 0xff;
    h->cspec.length = buffer[5] + buffer[6] * 0xff;
    h->cspec.entry_index = buffer[7];
    h->ispec.x_origin = buffer[8] + buffer[9] * 0xff;
    h->ispec.y_origin = buffer[10] + buffer[11] * 0xff;
    h->ispec.img_width = buffer[12] + buffer[13] * 0xff;
    h->ispec.img_height = buffer[14] + buffer[15] * 0xff; 
    h->ispec.img_depth = buffer[16];
    h->ispec.img_descriptor = buffer[17];
}

void tga_info( tga_fmt_t *h )
{
	printf( tga_info_01, h->id_length, h->color_map, h->img_type, h->cspec.entry_index, 
		h->cspec.length, h->cspec.entry_size, h->ispec.x_origin, h->ispec.y_origin,
		h->ispec.img_width, h->ispec.img_height, h->ispec.img_depth, h->ispec.img_descriptor );
}

int8 tga_load( FILE *f, image_t *img )
{
	tga_fmt_t h;
	uint8 buffer[sizeof(tga_fmt_t)];

	memset( &h, 0, sizeof(tga_fmt_t) );
	fread( &buffer, 1, sizeof(tga_fmt_t), f );
	buffer2tga( buffer, &h );
	tga_info( &h );
	return STATUS_SUCCESS;
}

int8 tga_save( FILE *f, image_t *img )
{
	return STATUS_SUCCESS;
}