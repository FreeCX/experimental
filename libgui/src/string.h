#ifndef __UI_STRING_H__
#define __UI_STRING_H__

#include <stdlib.h>

#define NULL_STR '\0'

size_t string_length( const char *src );
void string_copy( const char *src, char *dst );
void string_copy_n( const char *src, char *dst, size_t start, size_t length );

#endif
