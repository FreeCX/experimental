#include "string.h"
#include "token.h"

void tokenize( token_t **a, const char *data, const char *delimeters )
{
    size_t del_length = string_length( delimeters );
    size_t dat_length = string_length( data );
    size_t prev, next, n;
    size_t i, j;
    token_t *obj = NULL;

    obj = (token_t *) malloc( sizeof( token_t ) );
    obj->size = next = prev = 0;
    for ( i = 0; i < dat_length; i++ ) {
        for ( j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                if ( next - prev >= 1 ) {
                    obj->size++;
                }
                prev = next + 1;
            }
        }
        next++;
    }
    obj->name = ( char ** ) malloc( sizeof( obj->size ) * sizeof( char * ) );
    next = prev = n = 0;
    for ( i = 0; i < dat_length; i++ ) {
        for ( j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                if ( next - prev >= 1 ) {
                    obj->name[n] = ( char * ) malloc( ( next - prev + 1 ) * sizeof( char ) );
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

void free_token( token_t * data )
{
    size_t i;

    if ( data != NULL ) {
        for ( i = 0; i < data->size; i++ ) {
            free( data->name[i] );
        }
        free( data->name );
        free( data );
    }
}
