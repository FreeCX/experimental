#ifndef __UI_TOKEN_H__
#define __UI_TOKEN_H__

struct token {
    char *name;
    struct token *next;
};
typedef struct token token_t;

token_t *tokenize( char *data, const char *delimeters );
void free_token( token_t *data );

#endif