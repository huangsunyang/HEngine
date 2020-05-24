#ifndef __DRAW_COMMAND__
#define __DRAW_COMMAND__

#include <vector>
#include <string>
#include <initializer_list>

#include "GLObject/Program.hpp"
#include "GLObject/VertexArray.hpp"
#include "GLObject/VertexBuffer.hpp"
#include "GLObject/iMesh.hpp"
#include "GL/gl3w.h"

using namespace std;


class DrawCommand {
public:
    DrawCommand(): m_drawMode(GL_TRIANGLES) {m_program = new Program();}

    // buffer relative
    void initBuffers();
    VertexArray * getVertexArray() {return m_vao;}
    
    // shader relative
    void setShader(initializer_list<string> shaders);
    Program * getProgram() {return m_program;}

    // mesh info
    void loadMesh(string fileName);
    void loadGeometry(initializer_list<GLfloat> points);
    GLfloat * getPoints() {return m_mesh->getPoints();}
    GLuint * getIndices() {return m_mesh->getIndices();}
    size_t getPointNum() {return m_mesh->getPointNum();}
    size_t getIndiceNum() {return m_mesh->getIndiceNum();}

    // draw
    void draw();

protected:
    Program * m_program;
    VertexArray * m_vao;
    VertexBuffer * m_ebo;
    VertexBuffer * m_vbo;

    VertexInfo * m_vertexInfo;
    GLenum m_drawMode;              //绘制模式
    bool m_useIndice;               //是否使用索引绘制

    iMesh * m_mesh;    
};

#endif //__DRAW_COMMAND__
