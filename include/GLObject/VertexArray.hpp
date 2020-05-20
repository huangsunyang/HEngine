#ifndef __VERTEX_ARRAY_HPP__
#define __VERTEX_ARRAY_HPP__

#include <GL/gl3w.h>
#include <GLObject/VertexBuffer.hpp>

class VertexArray {
public:
    VertexArray();
    void activate();

    void setVertexBuffer(GLuint bindingIndex, VertexBuffer * vbo, GLuint stride);
    void setVertexBuffer(GLuint bindingIndex, VertexBuffer * vbo, GLuint offset, GLuint stride);
    void setVertexAttrib(GLuint attribIndex, GLuint size, GLuint type, GLuint relativeOffset);
    void bindVertexAttrib(GLuint attribIndex, GLuint bindingIndex);
    
    void enableAttrib(GLuint attribIndex);
    void disableAttrib(GLuint attribIndex);

    void setElementBuffer(VertexBuffer * vbo);

protected:
    GLuint m_vao;
};


#endif //__VERTEX_ARRAY_HPP__