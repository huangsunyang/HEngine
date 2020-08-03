#pragma once
#include <map>
#include <string>
#include <vector>
#include "GLObject/Texture.hpp"
#include "Freetype/FreeType.hpp"

using std::map;
using std::string;
using std::vector;

const int TEXTURE_SIZE = 1024;

struct Rect {
    Rect(int x, int y, int w, int h): startx(x), starty(y), width(w), height(h){}
    int startx;
    int starty;
    int width;
    int height;
    int offsetx;
    int offsety;
    float startxp;
    float startyp;
    float endxp;
    float endyp;
    int advancex;
};

/* Atlas is a collection of textures on one single large texture */
class FontAtlas {
public:
    FontAtlas(string fontName, int fontSize) {
        m_face = FreeTypeLibrary::instance()->loadFont(fontName, fontSize);
        auto metrics = m_face->handle()->size->metrics;
        m_penY = metrics.ascender >> 6;
        m_lineHeight = (metrics.ascender - metrics.descender) >> 6;

        m_texture = new Texture2D;
        m_texture->alloc(1, GL_R8, TEXTURE_SIZE, TEXTURE_SIZE);
    }
    void loadChar(char);
    Texture2D * getTexture() {return m_texture;}
    Rect getRectInfo(char ch) {return m_frameInfo.at(ch);}

protected:
    Texture2D * m_texture;
    map<char, Rect> m_frameInfo;
    FreeTypeFace * m_face;
    
    int m_ascender;
    int m_lineHeight;
    int m_penX = 0;
    int m_penY = 0;
};