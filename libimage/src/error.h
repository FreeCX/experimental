#ifndef __ERROR_H__
#define __ERROR_H__

#include "data.h"
#include <stdarg.h>
#include <stdio.h>

enum { LI_ERROR_NONE, LI_ERROR_OPEN_FILE, LI_ERROR_UNSUPPORTED_FORMAT };

char *img_status_msg(uint8_t status);
void img_send_error(const int error);
void img_module_error(const char *fmt, ...);

#endif