#include "data.h"

uint8 __DEBUG_FLAG__ = 0;

#ifdef ARCH_X86
inline uint16 bswap16( uint16 input )
{
	__asm( "xchgb %b0, %h0" : "=q" ( input ) : "0" ( input ) );
    return input;
}

inline uint32 bswap32( uint32 input )
{
#if __CPU__ > 386
	__asm( "bswap %0" : "=r" ( input ) :
#else
	__asm(
		"xchgb %b0, %h0\n"
		"rorl $16, %0\n"
		"xchgb %b0, %h0" :
		"=q" ( input ) :
#endif
	"0" ( input ) );
	return input;
}

inline uint64 bswap64( uint64 input )
{
	union { 
        uint64 ll;
        uint32 l[2]; 
    } w;

    w.ll = input;
	__asm(
		"xchgl %0, %1" :
      	"=r"( w.l[0] ), "=r"( w.l[1] ) :
    	"0"( bswap32( (unsigned long) input) ),
    	"1"( bswap32( (unsigned long)( input >> 32 ) ) ) 
    );
	return w.ll;
}
#else
inline uint16 bswap16( uint16 input )
{
	return ( ( input & 0xFF ) << 8 ) | ( input >> 8 );
}

inline uint32 bswap32( uint32 input )
{
	return ( ( input & 0xFF ) << 24 ) | ( ( input & 0xFF00 ) << 8 ) |
		( ( input & 0xFF0000 ) >> 8 ) | ( input >> 24 );
}

inline uint64 bswap64( uint64 input )
{
	union { 
        uint64 ll;
        uint32 l[2]; 
    } w, r;

    w.ll = input;
    r.l[0] = bswap32( w.l[1] );
    r.l[1] = bswap32( w.l[0] );
    return r.ll;
}
#endif

inline void swap8( uint8 *a, uint8 *b )
{
	uint8 t = *a;
	*a = *b;
	*b = t;
}

inline void swap16( uint16 *a, uint16 *b )
{
	uint16 t = *a;
	*a = *b;
	*b = t;
}

inline void swap32( uint32 *a, uint32 *b )
{
	uint32 t = *a;
	*a = *b;
	*b = t;
}

inline size_t fsize( FILE *f )
{
	size_t pos = ftell( f );
	size_t size;

	fseek( f, 0, SEEK_END );
	size = ftell( f );
	fseek( f, pos, SEEK_SET );
	return size;
}