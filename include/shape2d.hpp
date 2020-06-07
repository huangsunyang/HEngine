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
    size_t m_indiceNum;

    static HPolygon* from_vertex(vector<float> vec) {
        HPolygon * polygon = new HPolygon;

        size_t N = vec.size() / 3;
        polygon->N = N;
        polygon->m_indiceNum = N;
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

    static HPolygon* fromVertexIndice(vector<float> vec, vector<GLuint> index) {
        HPolygon * polygon = new HPolygon;

        size_t N = vec.size() / 3;
        polygon->N = N;
        polygon->m_indiceNum = index.size();
        polygon->verteces = new GLfloat[3 * N];
        polygon->indices = new GLuint[index.size()];
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

        for (int i = 0; i < index.size(); i++) {
            polygon->indices[i] = index[i];
        }

        return polygon;
    }

    static HPolygon* fromVertexCoord(vector<float> points, vector<float> coords) {
        HPolygon * polygon = new HPolygon;

        size_t N = points.size() / 3;
        polygon->N = N;
        polygon->m_indiceNum = N;
        polygon->verteces = new GLfloat[5 * N];
        polygon->indices = new GLuint[N];
        polygon->texcoord = nullptr; 

        polygon->vertexInfo = new VertexInfo;
        polygon->vertexInfo->useIndice = true;
        polygon->vertexInfo->attrInfos = vector<VertexAttrInfo> {
            {3, GL_FLOAT, 12},
            {2, GL_FLOAT, 8},
        };

        for (int i = 0; i < N; i++) {
            polygon->verteces[5 * i + 0] = points[3 * i + 0];
            polygon->verteces[5 * i + 1] = points[3 * i + 1];
            polygon->verteces[5 * i + 2] = points[3 * i + 2];
            polygon->verteces[5 * i + 3] = coords[2 * i + 0];
            polygon->verteces[5 * i + 4] = coords[2 * i + 1];
        }


        for (int i = 0; i < N; i++) {
            polygon->indices[i] = i;
        }

        return polygon;
    }

    virtual GLfloat * getTexcoord() {return texcoord;}
    virtual GLfloat * getPoints() {return verteces;}
    virtual GLuint * getIndices() {return indices;}
    virtual size_t getPointNum() {return N;}
    virtual size_t getIndiceNum() {return m_indiceNum;}
    virtual VertexInfo * getVertexInfo() {return vertexInfo;};
};


typedef HPolygon HTriangle;

#endif
