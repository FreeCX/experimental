#include "string.h"

void string_copy( const char *src, char *dst )
{
    size_t i = 0;

    while ( ( dst[i] = src[i] ) != NULL_STR ) {
        i++;
    }
}

void string_copy_n( const char *src, char *dst, size_t start, size_t length )
{
    size_t i = 0;

    while ( ( dst[i] = src[i+start] ) != NULL_STR && ++i <= length );
    dst[i] = NULL_STR;
}

size_t string_length( const char *src )
{
    size_t i = 1;

    while ( src[i++] != NULL_STR );
    return i;
}
