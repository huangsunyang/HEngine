#include "shader_mgr.h"
#include <string>
#include <fstream>
#include "logger.hpp"


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

GLuint ShaderMgr::get_location(const char * name) {
    GLuint ret = glGetUniformLocation(program, name);
    return ret;
}

ShaderMgr::~ShaderMgr()
{
    program = NULL;
}
