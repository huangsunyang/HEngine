#ifndef __SHAPE_2D__
#define __SHAPE_2D__
#include <initializer_list>
#include "sb7.h"
#include "vmath.h"
#include "shape_base.hpp"
#include "logger.hpp"

using std::initializer_list;

template <int N>
class HPolygon: public HShapeBase {
public:
    vmath::vec3 vertex[N];
    GLfloat verteces[3 * N];
    GLfloat texcoord[2 * N];
    GLuint indices[N];

    static HPolygon<N>* from_vertex(initializer_list<float> vec) {
        HPolygon<N> * polygon = new HPolygon<N>;

        int i = 0;
        for(auto x = vec.begin(); x != vec.end(); ++x, ++i) {
            polygon->verteces[i] = *x;
        }

        for (int i = 0; i < N; i++) {
            polygon->indices[i] = i;
        }

        return polygon;
    }

    virtual void setTexcoord(initializer_list<float> vec) {
        int i = 0;
        for(auto x = vec.begin(); x != vec.end(); ++x, ++i) {
            texcoord[i] = *x;
        }
    }

    virtual GLfloat * getTexcoord() {
        return texcoord;
    }

    virtual GLfloat * get_points() {
        return verteces;
    }

    virtual int get_point_num() {
        return N;
    }

    virtual GLuint * get_indices() {
        return indices;
    }
};


typedef HPolygon<3> HTriangle;

#endif
