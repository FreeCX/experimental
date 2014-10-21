#include "error.h"

static char ui_error_prefix[] = "[gui]: ";

void ui_error_send( const char *fmt, ... )
{
    va_list list;

    if ( fmt != NULL ) {
        printf( "%s", ui_error_prefix );
        va_start( list, fmt );
        vprintf( fmt, list );
        va_end( list );
    }
}
