#include <stdio.h>
#include <stdlib.h>

#define NULL_STR    '\0'

struct token {
    char **name;
    unsigned int size;
};
typedef struct token token_t;

char example_str[] = "special! text^= ?123;word @ number#2";
char example_del[] = "!^?;@# ";

void string_copy_n( char *src, char *dst, size_t start, size_t length )
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

void tokenize( token_t **a, char *data, const char *delimeters )
{
    size_t start = 0, counter = 0, length = 0, n = 0;
    size_t del_length = string_length( delimeters );
    size_t dat_length = string_length( data );
    size_t prev, next;
    size_t i, j;
    token_t *obj = NULL;

    obj = (token_t *) malloc( sizeof( token_t ) );
    obj->size = next = 0;
    prev = -1;
    for ( i = 0; i < dat_length; i++ ) {
        for ( j = 0; j < del_length; j++ ) {
            if ( data[i] == delimeters[j] ) {
                if ( next - prev > 1 ) {
                    obj->size++;
                }
                prev = next;
            }
        }
        next++;
    }
    obj->name = (char **) malloc( sizeof( obj->size ) * sizeof( char * ) );
    do {
        for ( i = 0; i < del_length; i++ ) {
            if ( data[counter] == delimeters[i] ) {
                length = counter - start;
                if ( length > 0 ) {
                    obj->name[n] = (char *) malloc( ( length + 1 ) * sizeof( char ) );
                    string_copy_n( data, obj->name[n], start, length-1 );
                    start = counter + 1;
                    n++;
                    break;
                }
                start = counter + 1;
            }
        }
    } while ( data[counter++] != NULL_STR ); 
    *a = obj;
}

void free_token( token_t *data )
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

int main( void )
{
    token_t *a;
    size_t i;

    tokenize( &a, example_str, example_del );
    for ( i = 0; i < a->size; i++ ) {
        printf( "token: %s\n", a->name[i] );
    }
    free_token( a );
    return 0;
}