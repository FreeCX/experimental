#include <iostream>
#include <unistd.h>
#include "simulator.hpp"

const float fric_coeff = 0.98f;

simulator::simulator( int count, int segment, float r, float dx, float dy )
{
    float ix = -dx, iy = -dy;
    int j = 0;

    srand( time( NULL ) );
    _pcount = count;
    x_size = dx;
    y_size = dy;
    _pobj.resize( _pcount );
    _segment = segment;
    _vertex = new float [2*_segment];
    for ( float i = 0; i <= 180.0f; i += ( 180.0f / ( _segment - 1 ) ) ) {
        _vertex[j++] = cos( 2 * i * M_PI / 179.0f );
        _vertex[j++] = sin( 2 * i * M_PI / 179.0f );
    }
    _vertex[j++] = 1.0f;
    _vertex[j++] = 0.0f;
    for ( int i = 0; i < _pcount; i++ ) {
        _pobj[i].p = vector2( ix, iy );
        _pobj[i].v = vector2( ( rand() % 100 - 50.0f ) / 10.0f, ( rand() % 100 - 50.0f ) / 10.0f );
        _pobj[i].a = vector2( 0.0f, -10.0f );
        _pobj[i].r = 2.0f;
        _pobj[i].solid = false;
        if ( iy == -dy || ix == dx || ix == -dx ) {
            _pobj[i].solid = true;
        } else {
            _pobj[i].r = 1.5f;
        }
        if ( ix >= dx ) {
            ix = -dx;
            iy += 4.0f;
        } else {
            ix += 4.0f;
        }
    }
}

simulator::~simulator()
{
    _pobj.clear();
    delete[] _vertex;
}

void simulator::collide( int i, int j )
{
    vector2 & v1 = _pobj[i].v, & v2 = _pobj[j].v;
    vector2 & p1 = _pobj[i].p, & p2 = _pobj[j].p;
    vector2 p = p2 - p1;
    vector2 n = p.norm();
    float d = ( _pobj[i].r + _pobj[j].r ) - p.length();

    if ( d >= 0 ) {
        float tmp = ( v1.length() + v2.length() ) * 0.5f;
        if ( _pobj[i].solid == false ) {
            p1 += -n * d;
            v1 = -n * tmp * fric_coeff;
        }
        if ( _pobj[j].solid == false ) {
            p2 += n * d;
            v2 =  n * tmp * fric_coeff;
        }
    }
}

void simulator::step( float dt )
{
    for ( int i = 0; i < _pcount; i++ ) {
        if ( _pobj[i].solid == false ) {
            _pobj[i].v += _pobj[i].a * dt;
            _pobj[i].p += _pobj[i].v * dt + _pobj[i].a * dt * dt * 0.5f;
        }
        for ( int j = i+1; j < _pcount; j++ ) {
            collide( i, j );
        }
    }
}

void simulator::draw( void )
{
    glLineWidth( 2.0f );
    for ( int i = 0; i < _pcount; i++ ) {
        if ( _pobj[i].solid ) {
            glColor3f( 1.0f, 1.0f, 1.0f );
        } else {
            glColor3f( 0.0f, 1.0f, 0.0f );
        }
        glPushMatrix();
        glEnableClientState( GL_VERTEX_ARRAY );
        glTranslatef( _pobj[i].p.x, _pobj[i].p.y, 0 );
        glScalef( _pobj[i].r, _pobj[i].r, 0 );
        glVertexPointer( 2, GL_FLOAT, 0, _vertex );
        glDrawArrays( GL_LINE_STRIP, 0, _segment + 1 );
        glDisableClientState( GL_VERTEX_ARRAY );
        glPopMatrix();
    }
}
