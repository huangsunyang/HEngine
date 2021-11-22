#pragma once
#include <map>
#include <string>
#include <vector>
#include "GLObject/Texture.hpp"
#include "Freetype/FreeType.hpp"
#include "glm/glm.hpp"

using std::map;
using std::string;
using std::vector;
using glm::vec2;

const int TEXTURE_SIZE = 512;

struct CharSizeInfo {
    // property of the font character
    int width;
    int height;
    int offsetx;
    int offsety;
    int advancex;
};

struct CharRectInfo {
    // pos in the altas
    float startxp;
    float startyp;
    float endxp;
    float endyp;
    int textureIndex;
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
    
    Texture2D * getTexture() {return m_texture;}
    CharSizeInfo getCharSizeInfo(char ch) {return m_fontInfo.at(ch);}
    CharRectInfo getCharRectInfo(char ch) {return m_fontRectInfo.at(ch);}
    
    void loadChar(char);

protected:
    Texture2D * m_texture;
    map<char, CharSizeInfo> m_fontInfo;
    map<char, CharRectInfo> m_fontRectInfo;
    
    FreeTypeFace * m_face;
    
    int m_ascender;
    int m_lineHeight;
    int m_penX = 0;
    int m_penY = 0;
};