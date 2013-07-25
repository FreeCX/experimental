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
		printf( "%x", h->colormap[i] );
	}
	putchar( '\n' );

	printf( pcx_info_02, h->reserved, h->nplanes, h->bpl, h->pallete_info,
		h->hscreen_size, h->vscreen_size );
	for ( i = 0; i < 54; i++ ) {
		printf( "%x", h->filler[i] );
	}
	putchar( '\n' );
}

int8 pcx_load( FILE *f, image_t *img )
{
	pcx_fmt_t h;
	uint32 xsize, ysize, total_bytes;
	uint8 *buffer, *data, count;
	uint32 i, n = 0;
	size_t file_size;

	memset( &h, 0, sizeof(pcx_fmt_t) );
	fread( &h, 1, sizeof(pcx_fmt_t), f );
	fseek( f, sizeof(pcx_fmt_t), SEEK_SET );
	pcx_info( &h );
	xsize = h.window.xmax - h.window.xmin + 1;
	ysize = h.window.ymax - h.window.ymin + 1;
	total_bytes = h.nplanes * h.bpl;
	file_size = fsize( f ) - sizeof(pcx_fmt_t);
	buffer = (uint8 *) malloc( file_size * sizeof(uint8) );
	fread( buffer, sizeof(uint8), file_size, f );
	data = (uint8 *) malloc( total_bytes * ysize  * sizeof(uint8) );
	img->bpp = h.bpp;
	img->width = xsize;
	img->height = ysize;
	while ( n < total_bytes * ysize ) {
		// 2 high bytes == 11
		if ( ( *buffer & 0xC0 ) == 0xC0 ) {
			// get last 6 bytes
			count = *buffer & 0x3F;
			buffer++;
		} else {
			count = 1;
		}
		for ( i = 0; i < count; i++ ) {
			data[i+n] = *buffer;
		}
		n += count;
		buffer++;
	}
	buffer = malloc( total_bytes * ysize  * sizeof(uint8) );
	for ( n = 0; n < ysize; n++ ) {
		for ( i = 0; i < xsize; i++ ) {
			buffer[3*(i+xsize*(ysize-n-1))+0] = data[i+(0+3*n)*xsize];
			buffer[3*(i+xsize*(ysize-n-1))+1] = data[i+(1+3*n)*xsize];
			buffer[3*(i+xsize*(ysize-n-1))+2] = data[i+(2+3*n)*xsize];
		}
	}
	img->data = buffer;
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