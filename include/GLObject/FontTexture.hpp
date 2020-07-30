#pragma once
#include "GLObject/Texture.hpp"
#include "FreeType/FreeType.hpp"
#include "sb7/vmath.h"

using vmath::vec2;

class FontTexture: public Texture2D {
public:
    void loadFont(string, char, int);
    vec2 getFontRect() {return {float(m_width), float(m_height)};}
    vec2 getFontOffset() {return {float(m_posx), float(m_posy)};}
    float getFontAdvance() {return float(m_advance >> 6);}
    virtual void bindTexture(int b) override {this->Texture2D::bindTexture(b); setTexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);};

protected:
    int m_advance;     // character gap, incluing self width
    int m_posx;        // leftup corner posx
    int m_posy;        // leftup corner posy
    int m_width;       // self width
    int m_height;      // self height
};