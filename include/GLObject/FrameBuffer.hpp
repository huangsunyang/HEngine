#pragma once
#include <GL/gl3w.h>
#include "GLObject/Texture.hpp"

class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();

    static void unbind(GLenum target);
    void bind(GLenum target);
    void bindTexture(GLenum attachment, Texture2D * texture, GLuint level);
    void drawBuffer(GLenum mode);

protected:
    GLuint m_fbo;
};