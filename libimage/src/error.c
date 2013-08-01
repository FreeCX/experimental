#include "error.h"

static char str_error[] = "[libimage]: %s\n";
static char *str_error_msg[] = {
	"No error",
	"Can't open file",
	"Unsupported file format"
};

void img_send_error( const int error )
{
    printf( str_error, str_error_msg[error] );
}

void img_module_error( const char *fmt, ... )
{
	char buffer[512];
	va_list ap;

	if ( fmt != NULL ) {
        va_start( ap, fmt );
        vsprintf( buffer, fmt, ap );
        va_end( ap );
        printf( str_error, buffer );
    }
}