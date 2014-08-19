#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdlib.h>

#define NULL_STR '\0'

struct token {
    char *name;
    struct token *next;
};
typedef struct token token_t;

token_t *tokenize( char *data, const char *delimeters );
void free_token( token_t *data );

#endif