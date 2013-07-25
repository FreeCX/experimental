#include "loader.h"

char *formats[] = {
	".bmp",
	".pcx",
	".tga"
};
uint8 format[] = { 
	FORMAT_NONE, FORMAT_BMP, FORMAT_PCX, FORMAT_TGA 
};
size_t fmt_size = sizeof(format);

void img_debug( uint8 param )
{
	if ( param ) {
		__DEBUG_FLAG__ = 1;
	} else {
		__DEBUG_FLAG__ = 0;
	}
}

uint8 img_load( char *filename, image_t *img )
{
	FILE *f;
	size_t i, select = 0;

	if ( ( f = fopen( filename, "r" ) ) == NULL ) {
		return STATUS_FAILED;
	}
	if ( __DEBUG_FLAG__ ) {
		printf( "loaded image: %s\n", filename );
	}
	for ( i = 1; i < fmt_size; i++ ) {
		if ( strstr( filename, formats[i-1] ) ) {
			select = i;
		}
	}
	switch ( select ) {
		case FORMAT_NONE:
			printf( "Unsupported file format!\n" );
			return STATUS_FAILED;
		case FORMAT_BMP:
			bmp_load( f, img );
			break;
		case FORMAT_PCX:
			pcx_load( f, img );
			break;
		case FORMAT_TGA:
			tga_load( f, img );
			break;
	}
	fclose( f );
	return STATUS_SUCCESS;
}