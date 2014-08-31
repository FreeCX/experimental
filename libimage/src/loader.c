#include "loader.h"

uint8 v_img_major = 0;
uint8 v_img_minor = 1;
uint8 v_img_build = 0; 
char  v_release[] = "alpha";

int8 ( *functions[] )( FILE *, image_t * ) = {
    bmp_load,
    pcx_load,
    gif_load,
    tga_load,
    img_null
};
const char *function_list[] = {
    "bmp",
    "pcx",
    "gif",
    "tga",
};
size_t fmt_size = sizeof( functions ) / sizeof( functions[0] );

void img_info( void )
{
    printf( "> libimage %d.%d.%d '%s'\n> build info: %s %s\n", v_img_major, v_img_minor, v_img_build, v_release, 
            __TIME__, __DATE__ );
}

void img_version( uint8 *major, uint8 *minor )
{
    *major = v_img_major;
    *minor = v_img_minor;
}

void img_debug( uint8 param )
{
    __DEBUG_FLAG__ = ( param == 1 ) ? 1 : 0;
}

int8 img_null( FILE *f, image_t *h )
{
    img_send_error( LI_ERROR_UNSUPPORTED_FORMAT );
    return STATUS_FAILED;
}

uint8 img_load( char *filename, image_t *img )
{
#ifdef __WIN32__
    static const char format_string[] = "> run %s function @ 0x%p: %s\n";
#elif __linux__
    static const char format_string[] = "> run %s function @ %p : %s\n";
#endif
    uint8 status;
    size_t i;
    FILE *f;

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
            printf( format_string, function_list[i], functions[i], img_status_msg( status ) );
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