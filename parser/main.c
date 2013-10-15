#include <stdio.h>
#include "parser_c.h"

int main( void )
{
	FILE *f;
	char **token = NULL;
	int count, opt, i;
	token_fmt_t params[] = {
		{ "MAX_COUNT", 0 },
		{ "RADIUS", 1 },
		{ "CHECK_ENERGY", 2 },
		{ "BORDERS", 3 },
		{ 0, 0 }
	};

	token = parser_init( " = ", 3, 32 );
	if ( ( f = fopen( "config.txt", "r" ) ) ) {
		do {
			opt = parser_get( f, params, &count );
			if ( opt > 0 ) {
				printf( "opt = %d\n", opt );
				printf( "count = %d\n", count );
				printf( "{ ");
				for ( i = 0; i < count; i++ ) {
					printf( "%s_{0x%08x}, ", token[i], token[i] );
				}
				printf( "}\n" );
			}
		} while ( opt != END_OF_FILE );
		fclose( f );
	}
}