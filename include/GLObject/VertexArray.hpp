#ifndef __VERTEX_ARRAY_HPP__
#define __VERTEX_ARRAY_HPP__

#include <GL/gl3w.h>

class VertexArray {
public:
    VertexArray();
    void activate();
    void bindAttribute();

protected:
    GLuint vao;
};


#endif //__VERTEX_ARRAY_HPP__