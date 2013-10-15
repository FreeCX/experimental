#include "parser_c.h"

char *delimeter = NULL;
int token_count = 0; 
int token_size_ = 0; 
char **token = NULL;

char ** parser_init( char *delimeter_symb, int max_token, int token_size )
{
	int i;

	delimeter = delimeter_symb;
	token_count = max_token;
	token_size_ = token_size;
	if ( token_count > 0 ) {
		token = (char **) calloc( token_count, sizeof(char *) );
		for ( i = 0; i < token_count; i++ ) {
			token[i] = (char *) calloc( token_size, sizeof(char) );
		}
	}
	return token;
}

int parser_get( FILE *f, token_fmt_t *t, int *count )
{
	int n, i;
	char buffer[BUFFER_MAX];

	if ( f == NULL ) {
		return ERROR_OPEN_FILE;
	}
	if ( delimeter == NULL ) {
		return ERROR_NULL_DELIMETER;
	} 
	if ( token_count == 0 ) {
		return ERROR_ZERO_TOKEN;
	}
	if ( feof( f ) ) {
		return END_OF_FILE;
	}
	memset( token, 0, token_count * token_size_ * sizeof(char) );
	fgets( buffer, BUFFER_MAX, f );
	if ( buffer[0] == '#' ) {
		return COMMENT_STRING;
	}
	token[0] = strtok( buffer, delimeter );
	if ( strlen( token[0] ) < 2 ) {
		return ZERO_STRING;
	} else {
		for ( n = 1; n < token_count; n++ ) {
			token[n] = strtok( 0, delimeter );
			if ( !token[n] ) {
				break;
			}
		}
		*count = n;
		for ( n = 0; t[n].name != NULL; n++ ) {
			if ( !strcmp( token[0], t[n].name ) ) {
				return t[n].number;
			}
		}
		return ERROR_UNKNOWN_TOKEN;
	}
}