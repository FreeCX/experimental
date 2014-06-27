#include <stdio.h>
#include <stdlib.h>

#define NULL_STR '\0'

struct token {
    char *name;
    struct token *next;
};
typedef struct token token_t;

char example_str[] = "special!text^=?123;word@number#2";
char example_del[] = "!^?;@#";

void string_copy( char *src, char *dst, size_t start, size_t length )
{
    size_t i = 0;

    while ( ( dst[i] = src[i+start] ) != NULL_STR && ++i <= length );
    /* if code is not working normal use this:
        while ( i <= length ) {
            dst[i] = src[i+start];
            i++;
        }
    */
    dst[i] = NULL_STR;
}

size_t string_length( const char *src )
{
    size_t i = 1;

    while ( src[i++] != NULL_STR );
    return i;
}

token_t *token_reverse( token_t *data )
{
    token_t *curr = data, *next;
    data = NULL;

    while ( curr != NULL ) {
        next = curr->next;
        curr->next = data;
        data = curr;
        curr = next;
    }
    return data;
}

token_t *tokenize( char *data, const char *delimeters )
{
    size_t start = 0, counter = 0, length;
    size_t i, del_length = string_length( delimeters );
    token_t *a = NULL, *b, *c, *head;

    do {
        for ( i = 0; i < del_length; i++ ) {
            if ( data[counter] == delimeters[i] ) {
                length = counter - start;
                b = (token_t *) malloc( sizeof(token_t) );
                b->name = (char *) malloc( length * sizeof(token_t) );
                string_copy( data, b->name, start, length-1 );
                b->next = a;
                a = b;
                start = counter + 1;
                break;
            }
        }
    } while ( data[counter++] != NULL_STR ); 
    return token_reverse( a );
}

void free_token( token_t *data )
{
    token_t *a = data, *b;

    while ( a != NULL ) {
        b = a;
        free( a->name );
        a = a->next;
        free( b );
    }
}

int main( void )
{
    token_t *a, *b;

    b = a = tokenize( example_str, example_del );
    while ( b != NULL ) {
        printf( "token: %s\n", b->name );
        b = b->next;
    }
    free_token( a );
}