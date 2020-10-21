#include "GLObject/FrameBuffer.hpp"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &m_fbo);
}

FrameBuffer::~FrameBuffer() {
    delete m_texture;
    glDeleteFramebuffers(1, &m_fbo);
}

void FrameBuffer::unbind(GLenum target) {
    glBindFramebuffer(target, 0);
}

void FrameBuffer::bind(GLenum target) {
    // GL_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER
    glBindFramebuffer(target, m_fbo);
}

void FrameBuffer::bindTexture(GLenum attachment, Texture2D * texture, GLuint level) {
    // GL_COLOR_ATTACHMENT0, GL_STENCIL_ATTACHMENT, GL_DEPTH_ATTACHMENT 
    glNamedFramebufferTexture(m_fbo, attachment, texture->getHandle(), level);
    m_texture = texture;
}

void FrameBuffer::drawBuffer(GLenum mode) {
    // GL_BACK, GL_COLOR_ATTACHMENT0
    glNamedFramebufferDrawBuffer(m_fbo, mode);
}

void FrameBuffer::drawBuffer(const vector<GLenum>& modes) {
    glNamedFramebufferDrawBuffers(m_fbo, modes.size(), modes.data());
}