#pragma once
#include <GL/gl3w.h>
#include "GLObject/Texture.hpp"
#include <vector>

using std::vector;

class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();

    static void unbind(GLenum target);
    void bind(GLenum target);
    void bindTexture(GLenum attachment, Texture2D * texture, GLuint level);
    void drawBuffer(GLenum mode);
    void drawBuffer(const vector<GLenum>& modes);
    Texture2D * getTexture() {return m_texture;}
    Texture2D * getDepthTexture() {return m_depthTexture;}

protected:
    GLuint m_fbo;
    Texture2D * m_texture;
    Texture2D * m_depthTexture;
};