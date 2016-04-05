#include <iostream>
#include <unistd.h>
#include "simulator.hpp"

const float fric_coeff = 1.0f;

simulator::simulator( int count, int segment, float r, float dx, float dy ) {
    float ix = -dx, iy = dy;
    int j = 0;

    srand( time( nullptr ) );
    x_size = dx;
    y_size = dy;
    _pobj.resize( count );
    _segment = segment;
    _vertex = new float [2*_segment];
    for ( float i = 0; i <= 180.0f; i += ( 180.0f / ( _segment - 1 ) ) ) {
        _vertex[j++] = cos( 2 * i * M_PI / 179.0f );
        _vertex[j++] = sin( 2 * i * M_PI / 179.0f );
    }
    _vertex[j++] = 1.0f;
    _vertex[j++] = 0.0f;
    for ( int i = 0; i < _pobj.size(); i++ ) {
        _pobj[i].p = vector2( ix, iy );
        _pobj[i].v = vector2( ( rand() % 100 - 50.0f ) / 5.0f, ( rand() % 100 - 50.0f ) / 5.0f );
        _pobj[i].a = vector2( 0.0f, -10.0f );
        _pobj[i].r = r;
        if ( ix >= dx ) {
            ix = -dx;
            iy -= 3.0f * r;
        } else {
            ix += 3.0f * r;
        }
    }
}

simulator::~simulator() {
    _pobj.clear();
    delete[] _vertex;
}

void simulator::add_line( vector2 p1, vector2 p2 ) {
    _lobj.push_back( { p1, p2 } );
}

void simulator::p_collide( int i, int j ) {
    vector2 & v1 = _pobj[i].v, & v2 = _pobj[j].v;
    vector2 & p1 = _pobj[i].p, & p2 = _pobj[j].p;
    vector2 p = p2 - p1;
    vector2 n = p.norm();
    float d = ( _pobj[i].r + _pobj[j].r ) - p.length();

    if ( d >= 0 ) {
        std::swap( v1, v2 );
        v1 *= fric_coeff;
        v2 *= fric_coeff;
        p1 -= n * d; 
        p2 += n * d;
    }
}

bool simulator::l_collide( int i, int j ) {
    // немного магии
    vector2 p1 = _lobj[i].p1 - _pobj[j].p;
    vector2 p2 = _lobj[i].p2 - _pobj[j].p;
    vector2 delta = p2 - p1;
    float a = delta.dot( delta );
    float b = 2.0f * p1.dot( delta );
    float c = p1.dot( p1 ) - powf( _pobj[j].r, 2.0f );
    if ( b > 0 ) {
        return c < 0;
    }
    if ( b > ( 2.0f * a ) ) {
        return ( 4.0f * a * c - powf( b, 2.0f ) ) < 0;
    }
    return a + b + c < 0;
}

void simulator::collider( int i ) {
    vector2 & p = _pobj[i].p;
    vector2 & v = _pobj[i].v;
    float & r = _pobj[i].r;
    float x1 = -x_size;
    float y1 = -y_size;
    float x2 = +x_size;
    float y2 = +y_size;

    if ( p.x < x1 + r ) {
        p.x = x1 + r;
    }
    if ( p.x > x2 - r ) {
        p.x = x2 - r;
    }
    if ( p.y < y1 + r ) {
        p.y = y1 + r;
    }
    if ( p.y > y2 - r ) {
        p.y = y2 - r;
    }
}

void simulator::step( float dt )
{
    for ( int i = 0; i < _pobj.size(); i++ ) {
        _pobj[i].v += _pobj[i].a * dt;
        _pobj[i].p += _pobj[i].v * dt + _pobj[i].a * dt * dt * 0.5f;
        for ( int j = i+1; j < _pobj.size(); j++ ) {
            p_collide( i, j );
        }
        collider( i );
        // for ( int j = 0; j < _lobj.size(); j++ ) {
        //     if ( l_collide( j, i ) ) {
        //         std::cout << "yes" << std::endl;
        //         _pobj[i].p = vector2( 0, 0 );
        //     }
        // }
    }
}

void simulator::draw( void )
{
    glLineWidth( 2.0f );
    glColor3f( 0.0f, 1.0f, 0.0f );
    for ( int i = 0; i < _pobj.size(); i++ ) {
        glPushMatrix();
        glEnableClientState( GL_VERTEX_ARRAY );
        glTranslatef( _pobj[i].p.x, _pobj[i].p.y, 0 );
        glScalef( _pobj[i].r, _pobj[i].r, 0 );
        glVertexPointer( 2, GL_FLOAT, 0, _vertex );
        glDrawArrays( GL_LINE_STRIP, 0, _segment + 1 );
        glDisableClientState( GL_VERTEX_ARRAY );
        glPopMatrix();
    }
    // glBegin( GL_LINES );
    // glColor3f( 1.0f, 0.0f, 0.0f );
    // for ( int i = 0; i < _lobj.size(); i++ ) {
    //     glVertex2f( _lobj[i].p1.x, _lobj[i].p1.y );
    //     glVertex2f( _lobj[i].p2.x, _lobj[i].p2.y );
    // }
    // glEnd();
}
