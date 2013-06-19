#ifndef __LOGIC__
#define __LOGIC__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR					256
#define STATUS_FINISHED			'-'
#define STATUS_IN_PROGRESS		'*'
#define STATUS_WAITING			'!'
#define STATIS_OTHER			'$'

struct list {
	char *org_name;
	char *rus_name;
	unsigned int series;
	char status;
	struct list *next;
};
typedef struct list list_t;

void strcopy( char *dest, char *source, int n, int m );
void list_parser( char str[], list_t *lst );
int list_load( char *name, list_t *lst );
int list_save( char *name, list_t *lst );
int list_copy( list_t *a, list_t *b );
int list_append( list_t *a, list_t *b );
list_t *list_search_by_name( list_t *lst, char *search );
list_t *list_search_by_status( list_t *lst, char status );
list_t *list_search_by_series( list_t *lst, int series );
char *list_status( char status );
int list_print( list_t *lst );
void list_clean( list_t *lst );
int list_html_out( char *name, list_t *lst );
list_t *list_reverse( list_t *lst );
	
#endif