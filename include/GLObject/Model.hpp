#ifndef __DRAW_COMMAND__
#define __DRAW_COMMAND__

#include "GLObject/Drawable.hpp"
#include "LogManager.hpp"

using namespace std;


class Model: public Drawable {
public:
    Model(): Drawable() {}
    virtual ~Model() {delete m_mesh; INFO("-------------- delete mesh\n");}

    // mesh info
    void loadMesh(string fileName);
    void loadVertex(const vector<GLfloat> &points);
    void loadVertexIndice(const vector<GLfloat> &points, const vector<GLuint> &indexs);
    void loadVertexCoord(const vector<GLfloat> &points, const vector<GLfloat> &coords);
    
    virtual GLfloat * getPoints() override {return m_mesh->getPoints();}
    virtual GLuint * getIndices() override {return m_mesh->getIndices();}
    virtual size_t getPointNum() override {return m_mesh->getPointNum();}
    virtual size_t getIndiceNum() override {return m_mesh->getIndiceNum();}
    virtual VertexInfo * getVertexInfo() override {return m_mesh->getVertexInfo();}

protected:
    iMesh * m_mesh;
};

#endif //__DRAW_COMMAND__
