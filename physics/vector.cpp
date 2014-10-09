#include "vector.hpp"

vector2::vector2()
{
}

vector2::vector2( const float x0, const float y0 ) : x( x0 ), y( y0 ) {};

std::ostream & operator << ( std::ostream &os, const vector2& v )
{
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}

void vector2::operator () ( const float x0, const float y0 )
{
    x = x0;
    y = y0;
}

bool vector2::operator == ( const vector2 &v )
{
    return x == v.x && y == v.y;
}

bool vector2::operator != ( const vector2 &v )
{
    return x != v.x || y != v.y;
}

vector2 & vector2::operator = ( const vector2 &v )
{
    x = v.x;
    y = v.y;
    return *this;
}

vector2 vector2::operator - ( void )
{
    return vector2( -x, -y );
}

vector2 vector2::operator + ( const vector2 &v )
{
    return vector2( x + v.x, y + v.y );
}

vector2 vector2::operator - ( const vector2 &v )
{
    return vector2( x - v.x, y - v.y );
}

vector2 vector2::operator * ( const float k )
{
    vector2 tmp( *this );
    return tmp *= k;
}

vector2 vector2::operator / ( const float k )
{
    vector2 tmp( *this );
    return tmp /= k;
}

vector2 vector2::operator -= ( const vector2 &v )
{
    x -= v.x;
    y -= v.y;
    return *this;
}

vector2 vector2::operator += ( const vector2 &v )
{
    x += v.x;
    y += v.y;
    return *this;
}

vector2 vector2::operator *= ( const float k )
{
    x *= k;
    y *= k;
    return *this;
}

vector2 vector2::operator /= ( const float k )
{
    x /= k;
    y /= k;
    return *this;
}

vector2 vector2::operator * ( const vector2 &v )
{
    return vector2( x * v.x, y * v.y );
}

vector2 vector2::abs( void )
{
    return vector2( fabsf( x ), fabsf( y ) );
}

vector2 vector2::norm( void )
{
    float n = sqrtf( x*x + y*y );
    float a = 0.0f;

    if ( n > FLT_EPSILON ) {
        a = 1.0f / n;
    }
    return vector2( x * a, y * a );
}

float vector2::length( void )
{
    return sqrtf( x*x + y*y );
}

float vector2::dot( const vector2 &v )
{
    return x*v.x + y*v.y;
}
