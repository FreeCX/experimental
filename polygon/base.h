#ifndef __POINT_H__
#define __POINT_H__

#include <cmath>

#define M_2PI 2*M_PI
#define EPS 1E-4

struct p2d {
	float x, y;
	struct p2d *next;
};
typedef struct p2d p2d_t;

class poly2d {
public:
	int n;
	dot2d *first;
};

class dot2d {
public:
	p2d_t p;
	void normalize( void );
	void angle_vector( float angle );
	float angle_of_vector( void );
}

float orient( p2d_t *a, p2d_t *b, p2d_t *c );
void rotate( float &x, float &y, float sinA, float cosA );
void rotate( float &x, float &y, float angle );

#endif