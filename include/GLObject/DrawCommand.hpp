#ifndef __DRAW_COMMAND__
#define __DRAW_COMMAND__

#include <map>
#include <vector>
#include <string>

#include "GLObject/Program.hpp"
#include "GLObject/VertexArray.hpp"
#include "GLObject/VertexBuffer.hpp"
#include "GLObject/Texture.hpp"
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
    void setShader(vector<string> shaders);
    Program * getProgram() {return m_program;}

    // texture relative
    void setTexture(vector<string> textureFiles);
    void setTexture(int bindingIndex, const char * textureFiles);

    // mesh info
    void loadMesh(string fileName);
    void loadVertex(vector<GLfloat> points);
    void loadVertexCoord(vector<GLfloat> points, vector<GLfloat> coords);
    GLfloat * getPoints() {return m_mesh->getPoints();}
    GLuint * getIndices() {return m_mesh->getIndices();}
    size_t getPointNum() {return m_mesh->getPointNum();}
    size_t getIndiceNum() {return m_mesh->getIndiceNum();}

    // mode relative
    void setDrawMode(GLenum mode) {m_drawMode = mode;}
    void setUseIndice(bool enable) {m_useIndice = enable;}

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
    map<int, Texture2D *> m_textures;
};

#endif //__DRAW_COMMAND__
