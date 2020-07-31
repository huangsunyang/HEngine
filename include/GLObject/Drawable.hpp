#pragma once

#include <map>
#include <vector>
#include <string>

#include "GLObject/Program.hpp"
#include "GLObject/VertexArray.hpp"
#include "GLObject/VertexBuffer.hpp"
#include "GLObject/Texture.hpp"
#include "GLObject/iMesh.hpp"
#include "GLObject/Transform.hpp"
#include "GL/gl3w.h"

using namespace std;


class Drawable {
public:
    Drawable(): 
        m_drawMode(GL_TRIANGLES),
        m_polygonMode(GL_FILL) 
    {
        m_program = new Program;
        m_transform = new Transform;
    }

    virtual ~Drawable() {
        delete m_vao;
        delete m_vbo;
        delete m_ebo;
        delete m_transform;
        delete m_program;
        for (auto pair: m_textures) {
            delete pair.second;
        }
        m_textures.clear();
    }

    // buffer relative
    virtual void initBuffers();
    VertexArray * getVertexArray() {return m_vao;}
    
    // shader relative
    void setShader(vector<string> shaders);
    Program * getProgram() {return m_program;}

    // texture relative
    void setTexture(vector<string> textureFiles);
    void setTexture(int bindingIndex, const char * textureFiles);

    // you have to implement these methods to draw something!!
    virtual GLfloat * getPoints() = 0;
    virtual GLuint * getIndices() = 0;
    virtual size_t getPointNum() = 0;
    virtual size_t getIndiceNum() = 0;
    virtual VertexInfo * getVertexInfo() = 0;

    // mode relative
    void setDrawMode(GLenum mode) {m_drawMode = mode;}
    void setPolygonMode(GLenum mode) {m_polygonMode = mode;}
    void setUseIndice(bool enable) {m_useIndice = enable;}

    // transform relative
    Transform * getTransform() {return m_transform;}
    vmath::mat4 getTransformMatrix() {return m_transform->getMatrix();}

    // draw
    virtual void draw();

protected:
    Program * m_program;
    VertexArray * m_vao;
    VertexBuffer * m_ebo;
    VertexBuffer * m_vbo;
    map<int, Texture2D *> m_textures;

    GLenum m_drawMode;              //绘制模式
    GLenum m_polygonMode;           //线框/填充？
    bool m_useIndice;               //是否使用索引绘制
    
    Transform * m_transform;
};;