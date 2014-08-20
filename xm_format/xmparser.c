#include <stdio.h>
#include <stdlib.h>
#include "xmparser.h"

uint8 hi_byte( uint16 a )
{
    return a >> 8;
}

uint8 lo_byte( uint16 a )
{
    return a & 0xFF;
}

xm_struct_t *load_xm( const char *filename )
{
    xm_struct_t *a = NULL;
    size_t read_size = 0;
    FILE *f;

    f = fopen( filename, "rb" );
    if ( f == NULL ) {
        return NULL;
    }
    a = (xm_struct_t *) malloc( sizeof( xm_struct_t ) );
    do {
        read_size += fread( a + read_size, 1, sizeof( xm_struct_t ) - read_size, f );
    } while ( read_size < sizeof( xm_struct_t ) );
    fclose( f );
    return a;
}

void print_xm( FILE *output, xm_struct_t *t )
{
    const char *flags_str[] = {
        "Amiga",
        "Linear"
    };
    const char xm_struct_info[] = 
        "id text:       '%.17s'\n"
        "module name:   '%.20s'\n"
        "stripped:      0x%x\n"
        "tracker name:  '%.20s'\n"
        "rev. number:   %d.%d\n"
        "header size:   %d\n"
        "song length:   %d pattern(s) [1..256]\n"
        "res. position: %d\n"
        "channels #:    %d [0..32/64]\n"
        "patterns #:    %d [1..256]\n"
        "instruments #: %d [1..128]\n"
        "flags:         %d [%s]\n"
        "default tempo: %d\n"
        "default bpm:   %d\n";

    fprintf( output, xm_struct_info, t->id_text, t->module_name, t->stripped, 
             t->tracker_name, hi_byte( t->revision_number ), 
             lo_byte( t->revision_number ), t->header_size, t->song_length, 
             t->restart_position, t->channels_number, t->patterns_number, 
             t->instrument_number, t->flags, flags_str[t->flags], 
             t->default_tempo, t->default_bpm );
    fprintf( output, "pattern table: " );
    for ( size_t i = 0; i < t->patterns_number; i++ ) {
        fprintf( output, "%02x ", t->pattern_table[i] );
    }
}

void close_xm( xm_struct_t *t )
{
    free( t );
}

int main( void )
{
    xm_struct_t *xm = NULL;

    xm = load_xm( "./EXPLOSiON.xm" );
    print_xm( stdout, xm );
    close_xm( xm );
    return 0;
}