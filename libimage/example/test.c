#include <stdio.h>
#include "../src/pcx.h"

int main( void )
{
	FILE *f;
	image_t img;

	f = fopen( "image.pcx", "r" );
	pcx_load( f, &img );
	fclose( f );
	return 0;
}