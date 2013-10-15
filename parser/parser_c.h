#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_MAX						256

enum {
	ERROR_OPEN_FILE = -10,
	ERROR_NULL_DELIMETER,
	ERROR_ZERO_TOKEN,
	ERROR_UNKNOWN_TOKEN,
	COMMENT_STRING,
	ZERO_STRING,
	END_OF_FILE = -1
};

struct token_fmt {
	char *name;
	int number;
};
typedef struct token_fmt token_fmt_t;

char ** parser_init( char *delimeter_symb, int max_token, int token_size );
int parser_get( FILE *f, token_fmt_t *t, int *count );

#endif