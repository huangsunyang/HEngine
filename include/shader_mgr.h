#pragma once
#include "sb7.h"	

class ShaderMgr
{
public:
    ShaderMgr();
    ~ShaderMgr();
    inline GLuint getProgram() { return program; }
    void bind_shader(GLenum shader_type, const char * file_name);
    void link_program();
    void delete_program();
    void use_program();

protected:
    GLuint program;
};

