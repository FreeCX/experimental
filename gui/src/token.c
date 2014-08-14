#include "string.h"
#include "token.h"

token_t *tokenize( char *data, const char *delimeters )
{
    size_t start = 0, counter = 0, length = 0;
    size_t del_length = string_length( delimeters );
    size_t d_length = string_length( data );
    size_t i, j, n;
    token_t *a = NULL;

    a = (token_t *) malloc( sizeof( token_t ) );
    a->size = 0;
    for ( i = 0; i < d_length; i++ ) {
        for ( j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                a->size++;
            }
        }
    }
    a->name = (char **) malloc( sizeof( a->size ) * sizeof( char * ) );
    do {
        for ( i = 0; i < del_length; i++ ) {
            if ( data[counter] == delimeters[i] ) {
                length = counter - start;
                if ( length > 0 ) {
                    a->name[n] = (char *) malloc( ( length + 1 ) * sizeof( char ) );
                    string_copy_n( data, a->name[n], start, length-1 );
                    start = counter + 1;
                    n++;
                    break;
                }
                start = counter + 1;
            }
        }
    } while ( data[counter++] != NULL_STR ); 
    return a;
}

void free_token( token_t *data )
{
    size_t i;

    for ( i = 0; i < data->size; i++ ) {
        free( data->name[i] );
    }
    free( data->name );
    free( data );
}