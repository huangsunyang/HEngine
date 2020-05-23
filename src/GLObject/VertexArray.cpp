#include "GLObject/VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vao);
}

void VertexArray::bindVertexArray() {
    glBindVertexArray(m_vao);
}

void VertexArray::setVertexBuffer(GLuint bindingIndex, VertexBuffer * vbo, GLuint stride) {
    /* setVertexBuffer is like to divide the whole buffer into small parts
     * each part is corresponding to a vertex with size of stride
     */
    setVertexBuffer(bindingIndex, vbo, 0, stride);
}

void VertexArray::setVertexBuffer(GLuint bindingIndex, VertexBuffer * vbo, GLuint offset, GLuint stride) {
    glVertexArrayVertexBuffer(m_vao, bindingIndex, vbo->getHandle(), offset, stride);
}

void VertexArray::setVertexAttrib(GLuint attribIndex, GLuint size, GLuint type, GLuint relativeOffset) {
    /* clarify inside a stride-length vertex, which is part of the attribute
     * normalized is set to GL_FALSE by default for convenient
     */
    glVertexArrayAttribFormat(m_vao, attribIndex, size, type, GL_FALSE, relativeOffset);
}

void VertexArray::bindVertexAttrib(GLuint attribIndex, GLuint bindingIndex) {
    glVertexArrayAttribBinding(m_vao, attribIndex, bindingIndex);
}

void VertexArray::enableAttrib(GLuint attribIndex) {
    glEnableVertexArrayAttrib(m_vao, attribIndex);
}

void VertexArray::disableAttrib(GLuint attribIndex) {
    glDisableVertexArrayAttrib(m_vao, attribIndex);
}

void VertexArray::setElementBuffer(VertexBuffer * vbo) {
    glVertexArrayElementBuffer(m_vao, vbo->getHandle());
}