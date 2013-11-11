#ifndef __DATA_H__
#define __DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glext.h>

typedef signed char int8;           // size = 1
typedef signed short int16;         // size = 2
typedef signed int int32;           // size = 4
typedef signed long int int64;      // size = 8

typedef unsigned char uint8;        // size = 1
typedef unsigned short uint16;      // size = 2
typedef unsigned int uint32;        // size = 4
typedef unsigned long int uint64;   // size = 8

#define STATUS_SUCCESS              0
#define STATUS_FAILED               1

#define IMG_RGB 					GL_RGB
#define IMG_BGR						GL_BGR
#define IMG_RGBA 					GL_RGBA
#define IMG_BGRA					GL_BGRA

struct image {
	uint32 c_format;
	uint8 *data;
	uint8 bpp;
	uint32 width, height;
};
typedef struct image image_t;

inline uint16 bswap16( uint16 input );
inline uint32 bswap32( uint32 input );
inline uint64 bswap64( uint64 input );
inline void swap8( uint8 *a, uint8 *b );
inline void swap16( uint16 *a, uint16 *b );
inline void swap32( uint32 *a, uint32 *b );
inline size_t fsize( FILE *f );

// inline uint16 lsb16( uint16 a );
// inline uint32 lsb32( uint32 a );
// inline uint64 lsb64( uint64 a );

#endif
