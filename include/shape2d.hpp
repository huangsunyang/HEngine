#ifndef __SHAPE_2D__
#define __SHAPE_2D__
#include "sb7/sb7.h"
#include "sb7/vmath.h"
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

    virtual ~HPolygon() {
        delete[] verteces;
        delete[] texcoord;
        delete[] indices;
        INFO("-----------------------delete HPolygon\n");
    };

    static HPolygon* from_vertex(const vector<float> &vec) {
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

        for (size_t i = 0; i < N; i++) {
            polygon->indices[i] = i;
        }

        return polygon;
    }

    static HPolygon* fromVertexIndice(const vector<float> &vec, const vector<GLuint> &index) {
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

        for (size_t i = 0; i < index.size(); i++) {
            polygon->indices[i] = index[i];
        }

        return polygon;
    }

    static HPolygon* fromVertexCoord(const vector<float> &points, const vector<float> &coords) {
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

        for (size_t i = 0; i < N; i++) {
            polygon->verteces[5 * i + 0] = points[3 * i + 0];
            polygon->verteces[5 * i + 1] = points[3 * i + 1];
            polygon->verteces[5 * i + 2] = points[3 * i + 2];
            polygon->verteces[5 * i + 3] = coords[2 * i + 0];
            polygon->verteces[5 * i + 4] = coords[2 * i + 1];
        }


        for (size_t i = 0; i < N; i++) {
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
