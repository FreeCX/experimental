#include "string.h"
#include "token.h"

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
    token_t *a = NULL, *b;

    do {
        for ( i = 0; i < del_length; i++ ) {
            if ( data[counter] == delimeters[i] ) {
                length = counter - start;
                if ( length > 0 ) {
                    b = (token_t *) malloc( sizeof(token_t) );
                    b->name = (char *) malloc( length * sizeof(token_t) );
                    string_copy_n( data, b->name, start, length-1 );
                    b->next = a;
                    a = b;
                    start = counter + 1;
                    break;
                }
                start = counter + 1;
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