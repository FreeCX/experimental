#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>

#define MAX_BUFFER		128
#define MAX_TOKEN    	10
#define TOKEN_SIZE	 	16

struct block {
	char *code;
	char *symb;
	struct block *next;
};
typedef struct block block_t;

int row, col;

block_t * decoder_add( block_t *a, char *b, char *c )
{
	block_t *blk;

	blk = (block_t *) calloc( 1, sizeof(block_t) );
	blk->code = (char *) calloc( strlen(b), sizeof(char) );
	strcpy( blk->code, b );
	blk->symb = (char *) calloc( strlen(c), sizeof(char) );
	strcpy( blk->symb, c );
	if ( a != NULL ) {
		blk->next = a;
		a = blk;
		return a;
	}
	return blk;
}

void decoder_clean( block_t *a )
{
	block_t *c;
	while ( a != NULL ) {
		c = a->next;
		free( a );
		a = c;
	} 
}

block_t * load_cmd( char *filename )
{
	block_t *list = NULL;
	char buffer[MAX_BUFFER], token0[TOKEN_SIZE], token1[TOKEN_SIZE];
	FILE *f;

	f = fopen( filename, "r" );
	if ( f != NULL ) {
		while ( !feof( f ) ) {
			fgets( buffer, MAX_BUFFER, f );
			strcpy( token0, strtok( buffer, " " ) );
			strcpy( token1, strtok( 0, " " ) );
			list = decoder_add( list, token0, token1 );
		}
		fclose( f );
	}
	return list;
}

int main( void )
{
	int i = 0, t, max = 0, y_pos = 0;
	char buffer[MAX_BUFFER], **token;
	block_t *list = NULL, *a = NULL;
	char delimeter[] = " \t";

	setlocale( LC_ALL, "" ) ;
	initscr();
	cbreak();
	move( 0, 0 );
	list = load_cmd( "cmd_list.txt" );
	a = list;
	token = (char **) calloc( MAX_TOKEN, sizeof(char *) );
	for ( t = 0; t < MAX_TOKEN; t++ ) {
		token[t] = (char *) calloc( TOKEN_SIZE, sizeof(char) );
	}
	while ( 1 ) {
		i = 0;
		while ( ( buffer[i++] = getch() ) != '\t' );
		move( y_pos, 0 );
		buffer[i] = 0;
		token[0] = strtok( buffer, delimeter );
		for ( t = 1; t < MAX_TOKEN; t++ ) {
			token[t] = strtok( 0, delimeter );
			if ( token[t] == NULL ) {
				break;
			}
		}
		max = t;
		for ( t = 0; t < max; t++ ) {
			a = list;
			if ( token[t][0] != '\\' ) {
				printw( "%s ", token[t] );
				refresh();
			} else {
				while ( a != NULL ) {
					if ( !strcmp( token[t], a->code ) ) {
						printw( "%s ", a->symb );
						refresh();
						break;
					}
					a = a->next;
				}
			}
		}
		printw( "\n" );
		refresh();
		y_pos++;
	}
	free( token );
	decoder_clean( list );
	endwin();
	return 0;
}