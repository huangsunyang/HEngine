#ifndef __DRAW_COMMAND__
#define __DRAW_COMMAND__

#include "GLObject/Drawable.hpp"

using namespace std;


class Model: public Drawable {
public:
    Model(): Drawable() {}

    // mesh info
    void loadMesh(string fileName);
    void loadVertex(vector<GLfloat> points);
    void loadVertexIndice(vector<GLfloat> points, vector<GLuint> indexs);
    void loadVertexCoord(vector<GLfloat> points, vector<GLfloat> coords);
    
    virtual GLfloat * getPoints() override {return m_mesh->getPoints();}
    virtual GLuint * getIndices() override {return m_mesh->getIndices();}
    virtual size_t getPointNum() override {return m_mesh->getPointNum();}
    virtual size_t getIndiceNum() override {return m_mesh->getIndiceNum();}
    virtual VertexInfo * getVertexInfo() override {return m_mesh->getVertexInfo();}

protected:
    iMesh * m_mesh;
};

#endif //__DRAW_COMMAND__
