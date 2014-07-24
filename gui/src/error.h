#ifndef __UI_ERROR_H__
#define __UI_ERROR_H__

#include <stdio.h>
#include <stdarg.h>
#include "data.h"

void ui_error_send( const char *fmt, ... );

#endif