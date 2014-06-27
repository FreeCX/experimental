#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <cmath>
#include <float.h>

class vector2 {
    friend std::ostream & operator << ( std::ostream &os, const vector2& v );
public:
    vector2();
    vector2( const float x0, const float y0 );
    void operator () ( const float x0, const float y0 );
    bool operator == ( const vector2 &v );
    bool operator != ( const vector2 &v );
    vector2 & operator = ( const vector2 &v );
    vector2 operator - ( void );
    vector2 operator + ( const vector2 &v );
    vector2 operator - ( const vector2 &v );
    vector2 operator * ( const float k );
    vector2 operator / ( const float k );
    vector2 operator -= ( const vector2 &v );
    vector2 operator += ( const vector2 &v );
    vector2 operator *= ( const float k );
    vector2 operator /= ( const float k );
    vector2 operator * ( const vector2 &v );
    vector2 abs( void );
    vector2 norm( void );
    float length( void );
    float dot( const vector2 &v );
    float x, y;
};

#endif