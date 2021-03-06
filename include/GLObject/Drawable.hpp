#pragma once

#include <map>
#include <vector>
#include <string>

#include "GLObject/Program.hpp"
#include "GLObject/VertexArray.hpp"
#include "GLObject/VertexBuffer.hpp"
#include "GLObject/Texture.hpp"
#include "GLObject/Transform.hpp"
#include "GL/gl3w.h"

using std::map;
using std::vector;

struct VertexAttrInfo {
    int num;
    GLenum type;   // type is an enum，GL_FLOAT but not GLfloat
    size_t size;   // equal to num * sizeof(Type)
};

struct VertexInfo {
    bool useIndice;                     // 是否使用索引绘制，即使用drawElements而不是drawArray
    vector<VertexAttrInfo> attrInfos;   // 每个属性的信息
};

class Drawable {
public:
    Drawable(): 
        m_drawMode(GL_TRIANGLES),
        m_polygonMode(GL_FILL) 
    {
        m_program = new Program;
        m_transform = new Transform;
        m_instanceCount = 1;
    }

    virtual ~Drawable() {
        delete m_vao;
        delete m_vbo;
        delete m_ebo;
        delete m_transform;
        delete m_program;
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
    void setTexture(vector<Texture2D *> textures);
    void setTexture(int bindingIndex, const char * textureFiles);

    // you have to implement these methods to draw something!!
    virtual GLfloat * getPoints() = 0;
    virtual GLuint * getIndices() = 0;
    virtual size_t getPointNum() = 0;
    virtual size_t getIndiceNum() = 0;
    virtual VertexInfo * getVertexInfo() = 0;

    // helper functions
    size_t getPointSize();

    // mode relative
    void setDrawMode(GLenum mode) {m_drawMode = mode;}
    void setPolygonMode(GLenum mode) {m_polygonMode = mode;}
    void setUseIndice(bool enable) {m_useIndice = enable;}
    void setInstanceCount(int count) {m_instanceCount = count;}

    // transform relative
    Transform * getTransform() {return m_transform;}
    glm::mat4 getTransformMatrix() {return m_transform->getMatrix();}

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
    int m_instanceCount;            //是否启用instance
    
    Transform * m_transform;
};