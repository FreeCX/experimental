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
    float r;
} point;

typedef struct {
    vector2 p1;
    vector2 p2;
} line;

class simulator {
public:
    simulator( int, int, float, float, float );
    ~simulator();
    void add_line( vector2, vector2 );
    void step( float );
    void draw( void );
private:
    void p_collide( int, int );
    bool l_collide( int, int );
    void collider( int );
    int _pcount;
    int _lcount;
    int _segment;
    std::vector< point > _pobj;
    std::vector< line > _lobj;
    float x_size;
    float y_size;
    float *_vertex;
};

#endif
