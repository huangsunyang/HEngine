#ifndef __GLOBJECT__PROGRAM__
#define __GLOBJECT__PROGRAM__

#include "sb7.h"
#include "GLObject/VertexBuffer.hpp"
#include <vector>
#include <string>

using namespace std;

class Program
{
public:
    Program();
    ~Program();
    inline GLuint getProgram() { return m_program; }
    void bindShader(GLenum shader_type, const char * file_name);
    void linkProgram();
    void deleteProgram();
    void activate();

    GLuint getLocation(const char * name);
    // vector<int> getLocations(GLsizei count, const char ** names, GLuint * indices);
    void setUniform(GLuint location, GLfloat value);

    GLuint getUniformBlockIndex(const GLchar * uniformBlockName);
    void bindUniformBlock(const GLchar * uniformBlockName, GLuint bindingIndex);
    void bindUniformBuffer(GLuint bindingIndex, VertexBuffer * buffer);

protected:
    GLuint m_program;
};

#endif //__GLOBJECT__PROGRAM__
