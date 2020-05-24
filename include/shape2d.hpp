#ifndef __SHAPE_2D__
#define __SHAPE_2D__
#include "sb7.h"
#include "vmath.h"
#include "GLObject/iMesh.hpp"
#include "logger.hpp"

using std::initializer_list;

class HPolygon: public iMesh {
public:
    GLfloat * verteces;
    GLfloat * texcoord;
    GLuint * indices;
    VertexInfo * vertexInfo;
    size_t N;

    static HPolygon* from_vertex(vector<float> vec) {
        HPolygon * polygon = new HPolygon;

        size_t N = vec.size();
        polygon->N = N;
        polygon->verteces = new GLfloat[3 * N];
        polygon->indices = new GLuint[N];
        polygon->texcoord = nullptr; 

        polygon->vertexInfo = new VertexInfo;
        polygon->vertexInfo->useIndice = true;
        polygon->vertexInfo->attrInfos = vector<VertexAttrInfo> {
            {3, GL_FLOAT, 12}
        };

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

    virtual GLfloat * getTexcoord() {return texcoord;}
    virtual GLfloat * getPoints() {return verteces;}
    virtual GLuint * getIndices() {return indices;}
    virtual size_t getPointNum() {return N;}
    virtual size_t getIndiceNum() {return N;}
    virtual VertexInfo * getVertexInfo() {return vertexInfo;};
};


typedef HPolygon HTriangle;

#endif
