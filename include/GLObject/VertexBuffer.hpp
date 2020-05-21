#ifndef __VERTEX_BUFFER_HPP__
#define __VERTEX_BUFFER_HPP__

#include <GL/gl3w.h>

class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();
    void bindTarget(GLenum target);

    void alloc(GLsizeiptr size, GLbitfield flags);
    void realloc(GLsizeiptr size, GLbitfield flags);
    
    void subData(const void * data);
    void subData(GLsizeiptr size, const void * data);
    void subData(GLintptr offset, GLsizeiptr size, const void * data);
    
    void * mapData(GLbitfield access);
    void * mapData(GLintptr offset, GLsizei size, GLbitfield access);
    void unmapData();
    void copyDataFrom(VertexBuffer & vb, GLintptr read_offset, GLintptr write_offset, GLsizeiptr size);
    void copyDataFrom(GLuint vbo, GLintptr read_offset, GLintptr write_offset, GLsizeiptr size);

    GLuint getHandle() {return m_vbo;}
    GLuint getSize() {return m_size;}
protected:
    GLuint m_vbo;
    GLsizeiptr m_size;
    GLbitfield m_flags;
};


#endif //__VERTEX_BUFFER_HPP__