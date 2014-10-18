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
    bool solid;
//    float m;
} point;

class simulator {
public:
    simulator( int, int, float, float, float );
    ~simulator();
    void step( float );
    void draw( void );
private:
    void collide( int, int );
    int _pcount;
    int _segment;
    std::vector< point > _pobj;
    float x_size;
    float y_size;
    float *_vertex;
};

#endif
