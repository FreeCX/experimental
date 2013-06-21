#include "logic.h"

list_t *base = NULL;

int main( int argc, char *argv[] )
{
	base = list_load( "anime-list" );
	list_print( base );
	free( base );
	return EXIT_SUCCESS;
}