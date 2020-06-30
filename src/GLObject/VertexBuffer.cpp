#include "GLObject/VertexBuffer.hpp"

VertexBuffer::VertexBuffer() {
    glCreateBuffers(1, &m_vbo);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_vbo);
}

void VertexBuffer::bindTarget(GLenum target) {
    /* in fact in opengl 4.5 where DSA methods like glNamedBufferSubData are introduced
     * glBindBuffer may not be called any more. even at old times, the [target] parameter
     * doesn't seem useful, though some types like GL_UNIFORM_BUFFER/GL_ARRAY_BUFFER exist
     */
    glBindBuffer(target, m_vbo);
}

void VertexBuffer::alloc(GLsizeiptr size, GLbitfield flags) {
    //by default, not data when allocate storage
    //glBufferStorage the first argument is target, not buffer
    // flags
    // GL_DYNAMIC_STORAGE_BIT set this bit to enable subData
    // GL_MAP_READ_BIT
    // GL_MAP_WRITE_BIT
    // GL_MAP_COHERENT_BIT
    // GL_MAP_PERSISTENT_BIT
    // GL_CLIENT_STORAGE_BIT
    m_size = size;
    m_flags = flags;
    glNamedBufferStorage(m_vbo, size, NULL, flags);
}

void VertexBuffer::alloc(GLsizeiptr size, GLbitfield flags, const void * data) {
    alloc(size, flags);
    subData(0, size, data);
}

void VertexBuffer::realloc(GLsizeiptr size, GLbitfield flags) {
    /* if you want to changs size of flags of a buffer
     * no way, you could only delete it and alloc a new one
     */
    if (size == m_size && m_flags == flags) {
        return;
    }
    glDeleteBuffers(1, &m_vbo);
    alloc(size, flags);
}

void VertexBuffer::realloc(GLsizeiptr size) {
    realloc(size, m_flags);
}

void VertexBuffer::subData(const void * data) {
    subData(0, m_size, data);
}

void VertexBuffer::subData(GLsizeiptr size, const void * data) {
    subData(0, size, data);
}

void VertexBuffer::subData(GLintptr offset, GLsizeiptr size, const void * data) {
    glNamedBufferSubData(m_vbo, offset, size, data);
}

void * VertexBuffer::mapData(GLbitfield access) {
    // glMapNamedBuffer(m_vbo, usage); we prefer glmapnamedbufferrange
    // access could be one of following, same as bit when call alloc
    // GL_MAP_READ_BIT
    // GL_MAP_WRITE_BIT
    // GL_MAP_COHERENT_BIT
    // GL_MAP_PERSISTENT_BIT
    return mapData(0, m_size, access);
}

void * VertexBuffer::mapData(GLintptr offset, GLsizei size, GLbitfield access) {
    return glMapNamedBufferRange(m_vbo, offset, size, access);
}

void VertexBuffer::unmapData() {
    glUnmapNamedBuffer(m_vbo);
}

void VertexBuffer::copyDataFrom(GLuint vbo, GLintptr read_offset, GLintptr write_offset, GLsizeiptr size) {
    glCopyNamedBufferSubData(vbo, m_vbo, read_offset, write_offset, size);
}

void VertexBuffer::copyDataFrom(VertexBuffer &vbo, GLintptr read_offset, GLintptr write_offset, GLsizeiptr size) {
    copyDataFrom(vbo.m_vbo, read_offset, write_offset, size);
}