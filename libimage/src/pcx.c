#include "pcx.h"

void pcx_info( pcx_fmt_t *h )
{
	int i;

	printf( "pcx.manufacturer = %u\n", h->manufacturer );
	printf( "pcx.version      = %u\n", h->version );
	printf( "pcx.endcoding    = %u\n", h->encoding );
	printf( "pcx.bpp          = %u\n", h->bpp );
	printf( "pcx.window       = %u %u %u %u\n", h->window[0], h->window[1], 
		h->window[2], h->window[3] );
	printf( "pcx.hdpi         = %u\n", h->hdpi );
	printf( "pcx.vdpi         = %u\n", h->vdpi );
	printf( "pcx.colormap     = " );
	for ( i = 0; i < 48; i++ ) {
		printf( "%u", h->colormap[i] );
	}
	putchar( '\n' );
	printf( "pcx.reserved     = %u\n", h->reserved );
	printf( "pcx.nplanes      = %u\n", h->nplanes );
	printf( "pcx.bpl          = %u\n", h->bpl );
	printf( "pcx.pallete_info = %u\n", h->pallete_info );
	printf( "pcx.hscreen_size = %u\n", h->hscreen_size );
	printf( "pcx.vscreen_size = %u\n", h->vscreen_size );
	printf( "pcx.filler       = " );
	for ( i = 0; i < 54; i++ ) {
		printf( "%u", h->filler[i] );
	}
	putchar( '\n' );
}

uint8 * pcx_load( FILE *f )
{
	pcx_fmt_t h;
	uint8 * buffer = NULL;

	memset( &h, 0, sizeof(pcx_fmt_t) );
	fread( &h, 1, sizeof(pcx_fmt_t), f );
	pcx_info( &h );
	return buffer;
}

void pcx_save( FILE *f, uint8 *data )
{
	/* input code */
}