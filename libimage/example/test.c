#include <stdio.h>
#include "../src/pcx.h"
#include "../src/tga.h"
#include "../src/bmp.h"

int main( void )
{
	FILE *f;
	image_t img;

	f = fopen( "image.pcx", "r" );
	pcx_load( f, &img );
	fclose( f );
	putchar( '\n' );
	f = fopen( "image.tga", "r" );
	tga_load( f, &img );
	fclose( f );
    putchar( '\n' );
    f = fopen( "image.bmp", "r" );
    bmp_load( f, &img );
    fclose( f );
	return 0;
}
