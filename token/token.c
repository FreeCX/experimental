#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NULL_STR    '\0'

struct token {
    char **name;
    size_t size;
};
typedef struct token token_t;

void string_copy_n( const char *src, char *dst, size_t start, size_t length )
{
    size_t i = 0;

    while ( ( dst[i] = src[i+start] ) != NULL_STR && i <= length ) { i++; }
    dst[i] = NULL_STR;
}

void tokenize( token_t **a, const char *data, const char *delimeters )
{
    size_t del_length = strlen( delimeters ) + 1;
    size_t dat_length = strlen( data ) + 1;
    size_t prev, next, n;
    token_t *obj = NULL;

    obj = (token_t *) malloc( sizeof( token_t ) );
    // get token count
    obj->size = next = prev = 0;
    for ( size_t i = 0; i < dat_length; i++ ) {
        for ( size_t j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                if ( next - prev >= 1 ) {
                    obj->size++;
                }
                prev = next + 1;
            }
        }
        next++;
    }
    obj->name = (char **) malloc( sizeof( obj->size ) * sizeof( char * ) );
    next = prev = n = 0;
    for ( size_t i = 0; i < dat_length; i++ ) {
        for ( size_t j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                if ( next - prev >= 1 ) {
                    obj->name[n] = (char *) malloc( ( next - prev + 1 ) * sizeof( char ) );
                    string_copy_n( data, obj->name[n], prev, next - prev - 1 );
                    prev = next + 1;
                    n++;
                    break;
                }
                prev = next + 1;
            }
        }
        next++;
    }
    *a = obj;
}

void free_token( token_t *data )
{
    if ( data != NULL ) {
        for ( size_t i = 0; i < data->size; i++ ) {
            free( data->name[i] );
        }
        free( data->name );
        free( data );
    }
}

int main( void )
{
    const char example_str[] = "special! text^= ?123;word @ number#2";
    const char example_del[] = "!^?;@# ";
    token_t *a;

    tokenize( &a, example_str, example_del );
    for ( size_t i = 0; i < a->size; i++ ) {
        printf( "token: %s\n", a->name[i] );
    }
    free_token( a );
    return 0;
}