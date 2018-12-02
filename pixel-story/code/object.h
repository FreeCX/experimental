#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "animation.h"

enum { OBJ_STAND = 0, OBJ_LEFT, OBJ_RIGHT, OBJ_STAND_ANIM, OBJ_LEFT_ANIM, OBJ_RIGHT_ANIM };

struct point {
    unsigned int x, y;
    int vx, vy, ax, ay;
};
typedef point point_t;

struct object {
    anim_file_t *a;
    point_t pos;
    unsigned int state;
};
typedef object object_t;

#endif