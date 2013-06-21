#include "logic.h"

char list_format[] = "%s # %s # %c %u\n";
char status[][64] = {
	"просмотр окончен",
	"в процессе просмотра",
	"в ожидании серий",
	"другое"
};
unsigned int list_count = 0;

void strcopy( char *dest, char *source, int n, int m )
{
	int i = 0;
	while ( n < m ) {
		dest[i] = source[n];
		n++; 
		i++;
	}
	dest[i] = '\0';
}

void list_parser( char str[], list_t *lst )
{
	list_t *a = lst;
	int len = strlen( str ), n[3], i = 0, j = 0;
	char buffer[16];

	while ( str[i] != '\n' ) {
		if ( str[i++] == '#' ) {
			n[j++] = i;
		}
	}
	a->org_name = malloc( (n[0]-2) * sizeof(char) );
	a->rus_name = malloc( (n[1]-2) * sizeof(char) );
	strcopy( a->org_name, str, 0, n[0]-2 );
	strcopy( a->rus_name, str, n[0]+1, n[1]-2 );
	a->status = str[n[1]+1];
	strcopy( buffer, str, n[1]+3, strlen( str ) );
	a->series = atoi( buffer );
}

list_t *list_load( char *name )
{
	FILE *f;
	char org_name[MAX_STR];
	char rus_name[MAX_STR];
	unsigned int series;
	char status, buffer[MAX_STR];
	list_t *l, *a = NULL;
	
	list_count = 0;
	if ( a != NULL ) {
		list_clean( a );
	}
	f = fopen( name, "r" );
	if ( f == NULL ) {
		return NULL;
	}
	while ( !feof( f ) ) {
		l = malloc( sizeof(list_t) );
		assert( l );
		memset( buffer, 0, MAX_STR * sizeof(char) );
		if ( fgets( buffer, MAX_STR, f ) != NULL ) {
			list_parser( buffer, l );
			l->next = a;
			a = l;
			a->index = list_count++;
		} else {
			free( l );
		}
	}
	fclose( f );
	return a;
}

int list_save( char *name, list_t *lst )
{
	FILE *f;
	list_t *a = lst, *l;

	f = fopen( name, "w" );
	if ( f == NULL ) {
		return EXIT_FAILURE;
	}
	l = list_reverse( a );
	while ( l != NULL ) {
		fprintf( f, list_format, l->org_name, l->rus_name, l->status, l->series );
		l = l->next;
	}
	fclose( f );
	return EXIT_SUCCESS;
}

int list_copy( list_t *a, list_t *b )
{
	strcpy( a->org_name, b->org_name );
	strcpy( a->rus_name, b->rus_name );
	a->status = b->status;
	a->series = b->series;
	return EXIT_SUCCESS;
}

list_t *list_append( list_t *a, list_t *b )
{
	list_t *l1 = a, *l2 = b, *l3 = NULL;

	while ( l2 != NULL ) {
		l3 = malloc( sizeof(list_t) );
		assert( l3 );
		memset( l3, 0, sizeof(list_t) );
		l3->org_name = malloc( strlen(l2->org_name) * sizeof(char) );
		l3->rus_name = malloc( strlen(l2->rus_name) * sizeof(char) );
		list_copy( l3, l2 );
		l3->index = l1->index + 1;
		l3->next = l1;
		l1 = l3;
		l2 = l2->next;
	}
	list_count = l1->index;
	return l1;
}

list_t *list_search_by_name( list_t *lst, char *search )
{
	list_t *a = lst, *b, *c;

	while ( a != NULL ) {
		if ( strstr( a->org_name, search ) || strstr( a->rus_name, search ) ) {
			b = malloc( sizeof(list_t) );
			assert( b );
			memset( b, 0, sizeof(list_t) );
			b->org_name = a->org_name;
			b->rus_name = a->rus_name;
			b->status = a->status;
			b->series = a->series;
			b->next = c;
			c = b;
		}
		a = a->next;
	}
	c->next = NULL;
	return c;
}

list_t *list_search_by_status( list_t *lst, char status )
{
	list_t *a = lst;
	return a;
}

list_t *list_search_by_series( list_t *lst, int series )
{
	list_t *a = lst;
	return a;
}

char *list_status( char stat )
{
	int i;

	switch ( stat ) {
		case '-':
			i = 0;
			break;
		case '*':
			i = 1;
			break;
		case '!':
			i = 2;
			break;
		case '$':
			i = 3;
			break;
	}
	return status[i];
}

int list_print( list_t *lst )
{
	char text_full[] = "Оригинальное название: '%s'\n"
				  "Русское название: '%s'\n"
				  "Статус: %s\n"
				  "Номер серии: %d\n\n";
	char text_mini[] = "Оригинальное название: '%s'\n"
				  "Русское название: '%s'\n"
				  "Статус: %s\n\n";
	list_t *a = lst;

	while ( a != NULL ) {
		printf( "index: %u\n", a->index );
		printf( "next:  %p\n", a->next );
		if ( a->series == 0 ) {
			printf( text_mini, a->org_name, a->rus_name, list_status( a->status ) );
		} else {
			printf( text_full, a->org_name, a->rus_name, list_status( a->status ), a->series );
		}
		a = a->next;
	}
	return EXIT_SUCCESS;
}

void list_clean( list_t *lst )
{
	list_t *c = lst;
	int count = 0;

    while ( c != NULL ) {
    	printf( "cound: %d\n", count );
        free( c->org_name );
        free( c->rus_name );
        free( c );
        c = c->next;
        count++;
    }
}

int list_html_out( char *file, list_t *lst )
{
	char out_format_01[] = "%04d: %s / %s [ %s ]\n<br>";
	char out_format_02[] = "%04d: %s / %s [ %s ][ Cерия %d ]\n<br>";
	char html_header[] = 
		"<html><head>"
		"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
		"<title>Список аниме</title></head><body>\n";
	char html_end[] = "</body></html>";
	list_t *a = lst;
	FILE *f;
	int i = 0;

	f = fopen( file, "w" );
	if ( f == NULL ) {
		return EXIT_FAILURE;
	}
	fprintf( f, "%s", html_header );
	while ( a != NULL ) {
		i++;
		if ( a->series == 0 ) {
			fprintf( f, out_format_01, i, a->org_name, a->rus_name, 
				list_status( a->status ) );
		} else {
			fprintf( f, out_format_02, i, a->org_name, a->rus_name, 
				list_status( a->status ), a->series );
		}
		a = a->next;
	}
	fprintf( f, "%s", html_end );
	fclose( f );
	return EXIT_SUCCESS;
}

list_t *list_reverse( list_t *lst )
{
	list_t *curr = lst, *next;
	lst = NULL;

	while ( curr != NULL ) {
		next = curr->next;
		curr->next = lst;
		lst = curr;
		curr = next;
	}
	return lst;
}