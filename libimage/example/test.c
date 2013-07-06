#include <stdio.h>
#include "../src/pcx.h"

int main( void )
{
	FILE *f;

	f = fopen( "image.pcx", "r" );
	pcx_load( f );
	fclose( f );
	return 0;
}