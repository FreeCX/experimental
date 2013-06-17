#include "logic.h"

list_t base;

int main( int argc, char *argv[] )
{
	list_load( "anime-list", &base );
	list_print( &base );
	// list_html_out( "anime-list.html", &base );
	return EXIT_SUCCESS;
}