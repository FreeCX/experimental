#ifndef __UI_TOKEN_H__
#define __UI_TOKEN_H__

#include <stdlib.h>

#define NULL_STR '\0'

struct token {
    char **name;
    unsigned int size;
};
typedef struct token token_t;

void tokenize( token_t **a, char *data, const char *delimeters );
void free_token( token_t *data );

#endif