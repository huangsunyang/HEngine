#include "GLObject/Program.hpp"
#include "GLObject/UniformBlock.hpp"
#include <string>
#include <fstream>
#include "logger.hpp"
#include "utils/FileUtils.hpp"

using namespace std;

Program::Program()
{
    m_program = glCreateProgram();
}

void Program::bindShader(vector<string> shader_paths) {
    for (auto shader: shader_paths) {
        bindShader(shader);
    }
}

void Program::bindShader(string shader_path) {
    string file_format_str = Utils::file_format(shader_path);
    GLenum shader_format;
    if (file_format_str == ".vs") {
        shader_format = GL_VERTEX_SHADER;
    } else if (file_format_str == ".fs") {
        shader_format = GL_FRAGMENT_SHADER;
    } else if (file_format_str == ".tesc") {
        shader_format = GL_TESS_CONTROL_SHADER;
    } else if (file_format_str == ".tese") {
        shader_format = GL_TESS_EVALUATION_SHADER;
    } else {
        INFO("---------------------------ERROR!!!!!!!!!!\n");
    }
    bindShader(shader_format, shader_path.c_str());
}

void Program::bindShader(GLenum shader_type, const char * file_name) {
    std::fstream t(file_name, std::ios::in);
    std::string ret((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    const GLchar* shader_source[] = {
        ret.c_str()
    };
    t.close();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, shader_source, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    INFO("Compile Shader %s %s!\n", file_name, status == GL_FALSE ? "Error": "Success");
    
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > 0) {
        char * message = new char[logSize];
        glGetShaderInfoLog(shader, logSize, nullptr, message);
        INFO("%s\n", message);
    }

    if (status == GL_FALSE) {
        glDeleteShader(shader);
        return;
    }

    glAttachShader(m_program, shader);
    glDeleteShader(shader);
}


void Program::linkProgram() {
    glLinkProgram(m_program);
}

void Program::useProgram() {
    glUseProgram(m_program);
    bindUniformBlock(UniformBlock::instance()->getBlockName().c_str(), UniformBlock::instance()->getBindingIndex());
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

void Program::setVec3Uniform(const char * name, const GLfloat * value) {
    GLuint location = getLocation(name);
    useProgram();
    return glUniform3fv(location, 1, value);
}

void Program::setVec4Uniform(const char * name, const GLfloat * value) {
    GLuint location = getLocation(name);
    useProgram();
    return glUniform4fv(location, 1, value);
}

void Program::setMatrix4fvUniform(const char * name, const GLfloat * value) {
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
    deleteProgram();
    m_program = NULL;
}
