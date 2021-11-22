#ifndef __SHAPE_2D__
#define __SHAPE_2D__
#include "sb7/sb7.h"
#include "glm/glm.hpp"
#include "utils/Logger.hpp"
#include "GLObject/Drawable.hpp"
#include <vector>

using std::initializer_list;

class HPolygon: public Drawable {
public:
    vector<GLfloat> m_verteces;
    vector<GLuint> m_indices;
    VertexInfo * m_vertexInfo;
    size_t m_vertexSize = 0;

    virtual ~HPolygon();

    HPolygon* loadVertexCoord(const vector<float> &points, const vector<float> &coords, const vector<GLuint> &index);

    virtual GLfloat * getPoints() {return m_verteces.data();}
    virtual GLuint * getIndices() {return m_indices.data();}
    virtual size_t getPointNum() {return m_verteces.size() / 3;}
    virtual size_t getIndiceNum() {return m_indices.size();}
    virtual VertexInfo * getVertexInfo() {return m_vertexInfo;};
};


typedef HPolygon HTriangle;

#endif
