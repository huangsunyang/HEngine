#pragma once
#include "GLObject/Texture.hpp"
#include "FreeType/FreeType.hpp"

class FontTexture: public Texture2D {
public:
    void loadFont(string, char);

protected:
    float m_advance;     // character gap, incluing self width
    float m_posx;        // leftup corner posx
    float m_posy;        // leftup corner posy
    float m_width;       // self width
    float m_height;      // self height
};