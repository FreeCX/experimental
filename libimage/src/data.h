#ifndef __DATA_H__
#define __DATA_H__

#include <stdio.h>
#include <string.h>

typedef signed char int8;			// size = 1
typedef signed short int16;			// size = 2
typedef signed int int32;			// size = 4
#ifdef __linux__
	typedef signed long int int64;
#else 
	typedef signed __int64 int64;
#endif								// size = 8

typedef unsigned char uint8;		// size = 1
typedef unsigned short uint16;		// size = 2
typedef unsigned int uint32;		// size = 4
#ifdef __linux__
	typedef unsigned long int uint64;
#else
	typedef unsigned __int64 uint64;
#endif								// size = 8

static inline uint16 bswap16( uint16 input );
static inline uint32 bswap32( uint32 input );
static inline uint64 bswap64( uint64 input );

#endif