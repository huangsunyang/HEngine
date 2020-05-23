#ifndef __SHAPE_BASE__
#define __SHAPE_BASE__
#include "vmath.h"
#include "sb7.h"

class HShapeBase {
public:
    void draw() {
        return;
    }

    virtual GLfloat * get_points() {return NULL;}
    virtual GLfloat * getTexcoord() {return NULL;}
    virtual GLuint * get_indices() {return NULL;}
    virtual int get_point_num() {return 0;}
    virtual bool is_visible() {return visible;}

    bool visible = true;
};

#endif
