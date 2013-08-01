#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>
#include <stdarg.h>

enum {
	LI_ERROR_NONE,
	LI_ERROR_OPEN_FILE,
	LI_ERROR_UNSUPPORTED_FORMAT
};

void img_send_error( const int error );
void img_module_error( const char *fmt, ... );

#endif