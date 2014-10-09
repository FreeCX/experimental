#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <GL/gl.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include "vector.hpp"

typedef struct {
    vector2 p;
    vector2 v;
    vector2 a;
//    float m;
} point;

typedef struct {
    vector2 p1;
    vector2 p2;
} line;

class simulator {
public:
    simulator( int, int, float, float, float );
    ~simulator();
    void setline( const vector2, const vector2 );
    void setline( float, float, float, float );
    void step( float );
    void draw( void );
private:
    void p_collide( int, int );
    void l_collide( int, int );
    int _pcount;
    int _lcount;
    int _segment;
    float _r;
    std::vector<point> _pobj;
    std::vector<line> _lobj;
    float *_vertex;
};

#endif
