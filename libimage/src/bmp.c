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
	uint16 image_size;

	memset( &h, 0, sizeof(bmp_fmt_t) );
	fread( &h, sizeof(bmp_fmt_t), 1, f );
	if ( __DEBUG_FLAG__ ) {
		bmp_info( &h );
	}
	image_size = h.file_size - h.img_offset;
	img->width = h.width;
	img->height = h.height;
	img->bpp = h.planes * h.bpp / 8;
	img->data = (uint8 *) malloc( image_size * sizeof(uint8) );
	fread( img->data, image_size, 1, f );
	switch ( h.compression ) {
		case 0: // BI_BGR
			// img->c_format = GL_BGR;
			break;
		case 1: // BI_RLE8
			break;
		case 2: // BI_RLE4
			break;
		case 3: // BI_BITFIELDS
			break;
		case 4: // BI_JPEG
			break;
		case 5: // BI_PNG
			break;
		case 6: // BI_ALPHABITFIELDS
			break;
		default:
			return STATUS_FAILED;
	}
	switch ( img->bpp ) {
		case 2:
			break;
		case 3:
			img->c_format = GL_RGB;
			break;
		case 4:
			img->c_format = GL_RGBA;
			break;
	}
	return STATUS_SUCCESS;
}

int8 bmp_save( FILE *f, image_t *img )
{
	return STATUS_SUCCESS;
}
