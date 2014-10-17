#include <iostream>
#include <unistd.h>
#include "simulator.hpp"

simulator::simulator( int count, int segment, float r, float dx, float dy )
{
    float ix = -dx, iy = -dy;
    int j = 0;

    srand( time( NULL ) );
    _pcount = count;
    _lcount = 0;
    _r = r;
    _pobj.resize( _pcount );
    _segment = segment;
    _vertex = new float [2*_segment];
    for ( float i = 0; i <= 180.0f; i += ( 180.0f / ( _segment - 1 ) ) ) {
        _vertex[j++] = cos( 2*i*M_PI / 179.0f );
        _vertex[j++] = sin( 2*i*M_PI / 179.0f );
    }
    _vertex[j++] = 1.0f;
    _vertex[j++] = 0.0f;
    for ( int i = 0; i < _pcount; i++ ) {
        _pobj[i].p = vector2( ix, iy );
        _pobj[i].v = vector2( (rand()%100 - 50.0f)/10.0f, (rand()%100 - 50.0f)/10.0f );
        _pobj[i].a = vector2( 0.0f, 0.0f );
        if ( ix >= dx ) {
            ix = -dx;
            iy += _r * 2.0f;
        } else {
            ix += _r * 2.0f;
        }
    }
}

simulator::~simulator()
{
    _pobj.clear();
    _lobj.clear();
    delete[] _vertex;
}

void simulator::setline( const vector2 p1, const vector2 p2 )
{
    _lcount++;
    _lobj.resize( _lcount );
    _lobj[_lcount-1].p1 = p1;
    _lobj[_lcount-1].p2 = p2;
}

void simulator::setline( float x0, float y0, float x1, float y1 )
{
    setline( vector2( x0, y0 ), vector2( x1, y1 ) );
}

void simulator::p_collide( int i, int j )
{
    vector2 & v1 = _pobj[i].v, & v2 = _pobj[j].v;
    vector2 & p1 = _pobj[i].p, & p2 = _pobj[j].p;
    vector2 p = p2 - p1;
    vector2 n = p.norm();
    float d = 2.0f * _r - p.length();

    if ( d >= 0 ) {
        p1 += -n * d;
        p2 +=  n * d;
        float tmp = v1.length();
        v1 = -n * v2.length() * fric_coeff;
        v2 =  n * tmp * fric_coeff;
    }
}

void simulator::l_collide( int i, int j )
{
    float A = _lobj[j].p2.y - _lobj[j].p1.y;
    float B = _lobj[j].p1.x - _lobj[j].p2.x;
    float C = -_lobj[j].p1.x * A -_lobj[j].p1.y * B;
    float d = fabsf( A*_pobj[i].p.x + B*_pobj[i].p.y + C ) / sqrtf( A*A + B*B );
    vector2 n = ( _lobj[i].p1 + _lobj[i].p2 ) * 0.5f;
    vector2 m = vector2( A, B ).norm() + n;
    vector2 k = n - vector2( A, B ).norm();

    if ( d <= 2.0f * _r ) {
        _pobj[i].v = -_pobj[i].v;
    }
}

void simulator::step( float dt )
{
    for ( int i = 0; i < _pcount; i++ ) {
        _pobj[i].v += _pobj[i].a * dt;
        _pobj[i].p += _pobj[i].v * dt + _pobj[i].a * dt * dt * 0.5f;
        for ( int j = i+1; j < _pcount; j++ ) {
            p_collide( i, j );
        }
        for ( int j = 0; j < _lcount; j++ ) {
            l_collide( i, j );
        }
    }
}

void simulator::draw( void )
{
    for ( int i = 0; i < _pcount; i++ ) {
        glPushMatrix();
        glEnableClientState( GL_VERTEX_ARRAY );
        glTranslatef( _pobj[i].p.x, _pobj[i].p.y, 0 );
        glScalef( _r, _r, 0 );
        glVertexPointer( 2, GL_FLOAT, 0, _vertex );
        glDrawArrays( GL_LINE_STRIP, 0, _segment + 1 );
        glDisableClientState( GL_VERTEX_ARRAY );
        glPopMatrix();
    }
    if ( _lcount > 0 ) {
        glLineWidth( 2.0f );
        glBegin( GL_LINES );
        glColor3f( 1.0f, 0.0f, 0.0f );
        for ( int i = 0; i < _lcount; i++ ) {
            glVertex2f( _lobj[i].p1.x, _lobj[i].p1.y );
            glVertex2f( _lobj[i].p2.x, _lobj[i].p2.y );
        }
        for ( int i = 0; i < _lcount; i++ ) {
            float A = _lobj[i].p2.y - _lobj[i].p1.y;
            float B = _lobj[i].p1.x - _lobj[i].p2.x;
            vector2 n = (_lobj[i].p2 + _lobj[i].p1) * 0.5;
            vector2 m = n - vector2( A, B ).norm();
            vector2 k = vector2( A, B ).norm() + n;

            glVertex2f( n.x, n.y );
            glVertex2f( m.x, m.y );
            glVertex2f( n.x, n.y );
            glVertex2f( k.x, k.y );
        }
        glEnd();
    }
}
