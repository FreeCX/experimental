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
	int i;

	printf( pcx_info_01, h->manufacturer, h->version, h->encoding, h->bpp, 
		h->window[0], h->window[1], h->window[2], h->window[3], h->hdpi,
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

	memset( &h, 0, sizeof(pcx_fmt_t) );
	fread( &h, 1, sizeof(pcx_fmt_t), f );
	pcx_info( &h );
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
	h.window[2] = img->width - 1;
	h.window[3] = img->height - 1;
	h.bpl = img->width;
	h.pallete_info = 1;
	/* input code */
	return STATUS_SUCCESS;
}