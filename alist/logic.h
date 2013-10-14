#ifndef __LOGIC__
#define __LOGIC__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR					256
enum {
    STATUS_FINISHED = 0,
    STATUS_IN_PROGRESS,
    STATUS_WAITING,
    STATUS_OTHER
};

struct list {
	char *org_name;
	char *rus_name;
	unsigned int series;
    unsigned int status;
	unsigned int index;
	struct list *next;
};
typedef struct list list_t;

struct element {
	unsigned int index;
	struct element *next;
};
typedef struct element elem_t;

void strcopy( char *dest, char *source, int n, int m );
void list_parser( char str[], list_t *lst );
list_t *list_load( char *name );
int list_save( char *name, list_t *lst );
void list_copy( list_t *a, list_t *b );
list_t *list_append( list_t *a, list_t *b );
elem_t *list_search_by_name( list_t *lst, char *search );
elem_t *list_search_by_status( list_t *lst, char status );
elem_t *list_search_by_series( list_t *lst, int series );
int list_print( list_t *lst );
void list_clean( list_t *lst );
list_t *list_reverse( list_t *lst );
char * list_status( int i );
	
#endif
