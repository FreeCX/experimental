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

void tga_info( tga_fmt_t *h )
{
	printf( tga_info_01, h->id_length, h->color_map, h->img_type, h->entry_index, 
		h->length, h->entry_size, h->x_origin, h->y_origin,
		h->img_width, h->img_height, h->img_depth, h->img_descriptor );
}

int8 tga_load( FILE *f, image_t *img )
{
	tga_fmt_t h;
	uint32 xsize, ysize, image_size, i;
	uint8 header[sizeof(tga_fmt_t)], depth;
	uint8 *data;

	memset( &h, 0, sizeof(tga_fmt_t) );
	fread( &h, sizeof(tga_fmt_t), 1, f );
	if ( __DEBUG_FLAG__ ) {
		tga_info( &h );
	}
	xsize = h.img_width;
	ysize = h.img_height;
	depth = h.img_depth;
	if ( depth != 8 && depth != 24 && depth != 32 ) {
		return EXIT_FAILURE;
	}
	depth /= 8;
	image_size = xsize * ysize * depth;
	switch ( h.img_type ) {
		case 0:
		case 1:
		case 2:
		case 3:
			data = (uint8 *) malloc( image_size * sizeof(uint8) );
			fread( data, image_size, 1, f );
			img->data = data;
			img->bpp = depth;
			img->width = xsize;
			img->height = ysize;
			break;
		default:
			img_module_error( "tga image type %u not supported", h.img_type );
			break;
	}
	switch ( depth ) {
		case 1:
    		img->c_format = GL_LUMINANCE8;
    		break;
    	case 3:
    		img->c_format = GL_BGR;
    		break;
    	case 4:
    		img->c_format = GL_RGBA;
    		break;
	}
	return STATUS_SUCCESS;
}

int8 tga_save( FILE *f, image_t *img )
{
	return STATUS_SUCCESS;
}