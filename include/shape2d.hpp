#ifndef __SHAPE_2D__
#define __SHAPE_2D__
#include "sb7.h"
#include "vmath.h"
#include "shape_base.hpp"

template <int N>
class HPolygon: public HShapeBase {
public:
    vmath::vec3 vertex[N];
    GLfloat verteces[3 * N];

    static HPolygon<N>* from_vertex(double first, ...) {
        HPolygon<N> * polygon = new HPolygon<N>;
        va_list ap;
        int n = 3 * N;
        va_start(ap, n);
        polygon->verteces[0] = first;
        for(int i = 1; i < n; ++i) {
            polygon->verteces[i] = va_arg(ap, double);
        }
        va_end(ap);
        return polygon;
    }

    virtual GLfloat * get_points() {
        return verteces;
    }

    virtual int get_triangle_num() {
        return N;
    }
};


typedef HPolygon<3> HTriangle;
#endif
