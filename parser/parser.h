#ifndef _CONF_H
#define _CONF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    int number;  
} p_format;

enum
{
    S_TEXT,
    S_COMMENT,
    S_DATA
};

typedef struct {
    int max_count;
    float radius;
    int borders;
    int interaction;
    int ext_interaction;
    int gravity_dim;
    float G;
    float E;
    int hit_type;
    int start_speed;
    int check_energy;
} rc;

#define T_COMMENT           '#'
#define T_SPACE             ' '
#define T_END               '\n'
#define T_DATA              '='
#define T_NULL              '\0'

int get_parameter(FILE *f, p_format *p, char *a);
rc configure(char *file);

#endif
