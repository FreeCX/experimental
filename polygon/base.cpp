#include "base.h"

void dot2d::normalize( void )
{
	float l = sqrtf( p.x * p.x + p.y * p.y );
	if ( fabs( l ) < EPS ) {
		p.x /= l;
		p.y /= l;
	}
}

void dot2d::angle_vector( float angle )
{
	p.x = cosf( angle );
	p.y = sinf( angle );
}

float dot2d::angle_of_vector( void )
{
	float a = acosf( p.x );
	if ( p.y < 0 ) {
		a = M_2PI - a;
	}
	return a;
}

float orient( p2d_t *a, p2d_t *b, p2d_t *c )
{
	return ( a->x - c->x ) * ( b->y - c->y ) - 
		( a->y - c->y ) * ( b->x - c->x );
}

void rotate( float &x, float &y, float sinA, float cosA )
{
	float xx = x * cosA - y * sinA;
	y = y * cosA + x * sinA;
	x = xx;
}

void rotate( float &x, float &y, float angle )
{
	rotate( x, y, sinf( angle ), cosf( angle ) );
}