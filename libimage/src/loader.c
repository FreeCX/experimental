#include "loader.h"

int8 ( *functions[] )( FILE *, image_t * ) = {
    bmp_load,
    pcx_load,
    gif_load,
    tga_load,
    img_null
};
size_t fmt_size = sizeof( functions ) / 8;

void img_debug( uint8 param )
{
    if ( param ) {
        __DEBUG_FLAG__ = 1;
    } else {
        __DEBUG_FLAG__ = 0;
    }
}

int8 img_null( FILE *f, image_t *h )
{
    img_send_error( LI_ERROR_UNSUPPORTED_FORMAT );
    return STATUS_FAILED;
}

uint8 img_load( char *filename, image_t *img )
{
    FILE *f;
    size_t i;
    uint8 status;

    if ( ( f = fopen( filename, "r" ) ) == NULL ) {
        img_send_error( LI_ERROR_OPEN_FILE );
        return STATUS_FAILED;
    }
    if ( __DEBUG_FLAG__ ) {
        printf( "image: %s\n", filename );
    }
    for ( i = 0; i < fmt_size; i++ ) {
        status = functions[i]( f, img );
        if ( __DEBUG_FLAG__ && status != STATUS_FAILED ) {
            printf( " > [%lu] @ 0x%p : %s\n", i, functions[i], 
                img_status_msg( status ) );
        }
        if ( status == STATUS_SUCCESS ) {
            break;
        }
    }
    fclose( f );
    return status;
}

uint8 img_close( image_t *img )
{
    free( img->data );
    return EXIT_SUCCESS;
}