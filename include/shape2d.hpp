#ifndef __SHAPE_2D__
#define __SHAPE_2D__
#include "sb7/sb7.h"
#include "glm/glm.hpp"
#include "logger.hpp"
#include "GLObject/Drawable.hpp"

using std::initializer_list;

class HPolygon: public Drawable {
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
        delete vertexInfo;
    };

    HPolygon* loadVertex(const vector<float> &vec) {
        HPolygon * polygon = this;

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

        polygon->initBuffers();
        return polygon;
    }

    HPolygon* loadVertexIndice(const vector<float> &vec, const vector<GLuint> &index) {
        HPolygon * polygon = this;

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

        polygon->initBuffers();
        return polygon;
    }

    HPolygon* loadVertexCoord(const vector<float> &points, const vector<float> &coords, const vector<GLuint> &index) {
        HPolygon * polygon = this;

        size_t N = points.size() / 3;
        this->N = N;

        int vertex_size = 3;
        if (!coords.empty()) vertex_size += 2;

        this->verteces = new GLfloat[vertex_size * N];
        if (index.empty()) {
            this->m_indiceNum = N;
            this->indices = new GLuint[N];
            for (size_t i = 0; i < N; i++) {
                this->indices[i] = i;
            }
        } else {
            this->m_indiceNum = index.size();
            this->indices = new GLuint[index.size()];
            for (size_t i = 0; i < index.size(); i++) {
                this->indices[i] = index[i];
            }
        }
        this->texcoord = nullptr; 

        this->vertexInfo = new VertexInfo;
        this->vertexInfo->useIndice = true;
        auto attrInfos = vector<VertexAttrInfo> {
            {3, GL_FLOAT, 12},
        };
        if (!coords.empty()) attrInfos.push_back({2, GL_FLOAT, 8});
        this->vertexInfo->attrInfos = attrInfos;

        for (size_t i = 0; i < N; i++) {
            this->verteces[vertex_size * i + 0] = points[3 * i + 0];
            this->verteces[vertex_size * i + 1] = points[3 * i + 1];
            this->verteces[vertex_size * i + 2] = points[3 * i + 2];
            if (!coords.empty()) {
                this->verteces[vertex_size * i + 3] = coords[2 * i + 0];
                this->verteces[vertex_size * i + 4] = coords[2 * i + 1];
            }
        }

        this->initBuffers();
        return this;
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
