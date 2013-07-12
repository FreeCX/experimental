#include "pcx.h"

char pcx_info_01[] = 
	"pcx.manufacturer = %u\n"
	"pcx.version      = %u\n"
	"pcx.endcoding    = %u\n"
	"pcx.bpp          = %u\n"
	"pcx.window       = %u %u %u %u\n"
	"pcx.hdpi         = %u\n"
	"pcx.vdpi         = %u\n"
	"pcx.colormap     = ";
char pcx_info_02[] = 
	"pcx.reserved     = %u\n"
	"pcx.nplanes      = %u\n"
	"pcx.bpl          = %u\n"
	"pcx.pallete_info = %u\n"
	"pcx.hscreen_size = %u\n"
	"pcx.vscreen_size = %u\n"
	"pcx.filler       = ";

void pcx_info( pcx_fmt_t *h )
{
	uint8 i;

	printf( pcx_info_01, h->manufacturer, h->version, h->encoding, h->bpp, 
		h->window.xmin, h->window.ymin, h->window.xmax, h->window.ymax, h->hdpi,
		h->vdpi );
	for ( i = 0; i < 48; i++ ) {
		printf( "%u", h->colormap[i] );
	}
	putchar( '\n' );

	printf( pcx_info_02, h->reserved, h->nplanes, h->bpl, h->pallete_info,
		h->hscreen_size, h->vscreen_size );
	for ( i = 0; i < 54; i++ ) {
		printf( "%u", h->filler[i] );
	}
	putchar( '\n' );
}

int8 pcx_load( FILE *f, image_t *img )
{
	pcx_fmt_t h;
	uint32 xsize, ysize, total_bytes;
	uint8 *buffer, *data, count;
	uint32 i = 0, j, n = 0, k;

	memset( &h, 0, sizeof(pcx_fmt_t) );
	fread( &h, 1, sizeof(pcx_fmt_t), f );
	fseek( f, sizeof(pcx_fmt_t), SEEK_SET );
	pcx_info( &h );
	xsize = h.window.xmax - h.window.xmin + 1;
	ysize = h.window.ymax - h.window.ymin + 1;
	total_bytes = h.nplanes * h.bpl;
	buffer = (uint8 *) malloc( total_bytes * ysize * sizeof(uint8) );
	fread( buffer, 1, total_bytes * ysize * sizeof(uint8), f );
	data = (uint8 *) malloc( xsize * ysize * h.nplanes * sizeof(uint8) );
	img->bpp = h.bpp;
	img->width = xsize;
	img->height = ysize;
	while ( i < total_bytes * ysize ) {
		// 2 high bytes == 11
		if ( buffer[i] >> 8 == 12 ) {
			// get last 6 bytes
			count = buffer[i] & 0x3F;
			for ( j = 0; j < count; j++ ) {
				data[n+j] = buffer[i+1];
				// printf( "%x", data[n+j] );
			}
			n += count;
		} else {
			data[n] = buffer[i];
			// printf( "%x", data[n] );
		}
		n++; i++;
	}
	img->data = data;
	return STATUS_SUCCESS;
}

int8 pcx_save( FILE *f, image_t *img )
{
	pcx_fmt_t h;

	memset( &h, 0, sizeof(pcx_fmt_t) );
	h.manufacturer = 10;
	h.version = 5;
	h.encoding = 1;
	h.bpp = img->bpp;
	h.window.xmax = img->width - 1;
	h.window.ymax = img->height - 1;
	h.bpl = img->width;
	h.pallete_info = 1;
	/* input code */
	return STATUS_SUCCESS;
}