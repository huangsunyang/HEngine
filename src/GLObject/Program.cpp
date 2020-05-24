#include "GLObject/Program.hpp"
#include <string>
#include <fstream>
#include "logger.hpp"
#include "utils/FileUtils.hpp"

using namespace std;

Program::Program()
{
    m_program = glCreateProgram();
}

void Program::bindShader(string shader_path) {
    string file_format_str = Utils::file_format(shader_path);
    GLenum shader_format;
    if (file_format_str == ".vs") {
        shader_format = GL_VERTEX_SHADER;
    } else if (file_format_str == ".fs") {
        shader_format = GL_FRAGMENT_SHADER;
    }
    bindShader(shader_format, shader_path.c_str());
}

void Program::bindShader(GLenum shader_type, const char * file_name) {
    std::ifstream t(file_name);
    std::string ret((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    const GLchar* shader_source[] = {
        ret.c_str()
    };

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, shader_source, NULL);
    glCompileShader(shader);
    glAttachShader(m_program, shader);
    //glDeleteShader(shader);
}


void Program::linkProgram() {
    glLinkProgram(m_program);
}

void Program::useProgram() {
    glUseProgram(m_program);
}

void Program::deleteProgram() {
    glDeleteProgram(m_program);
}

GLuint Program::getLocation(const char * name) {
    GLuint ret = glGetUniformLocation(m_program, name);
    return ret;
}

// vector<int> Program::getLocations(GLsizei count, const char ** names, GLuint * indices) {
    // glGetUniformIndices(m_program, count, names, indices);
// }

void Program::setIntUniform(const char * name, GLint value) {
    GLuint location = getLocation(name);
    useProgram();
    glUniform1i(location, value);
}

void Program::setFloatUniform(const char * name, GLfloat value) {
    GLuint location = getLocation(name);
    useProgram();
    glUniform1f(location, value);
}

void Program::setMatrix4fvUniform(const char * name, GLfloat * value) {
    GLuint location = getLocation(name);
    useProgram();
    glUniformMatrix4fv(location, 1, false, value);
}

GLuint Program::getUniformBlockIndex(const GLchar * uniformBlockName) {
    return glGetUniformBlockIndex(m_program, uniformBlockName);
}

void Program::bindUniformBlock(const GLchar * blockName, GLuint bindingIndex) {
    GLuint uniformBlockIndex = getUniformBlockIndex(blockName);
    glUniformBlockBinding(m_program, uniformBlockIndex, bindingIndex);
}

void Program::bindUniformBuffer(GLuint bindingIndex, VertexBuffer * buffer) {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, buffer->getHandle());
}

Program::~Program()
{
    m_program = NULL;
}