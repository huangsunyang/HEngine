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
    virtual bool is_visible() {return visible;}
    virtual int get_triangle_num() {return 0;}

    bool visible = true;
};

#endif
