#include "shader_mgr.h"
#include <string>
#include <fstream>


ShaderMgr::ShaderMgr()
{
    program = glCreateProgram();
}


void ShaderMgr::bind_shader(GLenum shader_type, const char * file_name) {
    std::ifstream t(file_name);
    std::string ret((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    const GLchar* shader_source[] = {
        ret.c_str()
    };

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, shader_source, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
    //glDeleteShader(shader);
}


void ShaderMgr::link_program() {
    glLinkProgram(program);
}

void ShaderMgr::use_program() {
    glUseProgram(program);
}

void ShaderMgr::delete_program() {
    glDeleteProgram(program);
}

ShaderMgr::~ShaderMgr()
{
    program = NULL;
}
