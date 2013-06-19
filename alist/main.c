#include "logic.h"

list_t base;

int main( int argc, char *argv[] )
{
	list_load( "anime-list", &base );
	list_print( &base );
	return EXIT_SUCCESS;
}