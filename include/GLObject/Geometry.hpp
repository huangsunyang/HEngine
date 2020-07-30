#pragma once
#include "GLObject/Model.hpp"
#include "sb7/vmath.h"
#include "GLObject/FontTexture.hpp"


using vmath::vec2;


class Box: public Model {
public:
    Box(float length, float width, float height) {
        // from bottom to up, from first area to fourth area
        loadVertexIndice({
            length / 2, width / 2, -height / 2,
            -length / 2, width / 2, -height / 2,
            -length / 2, -width / 2, -height / 2,
            length / 2, -width / 2, -height / 2,
             length / 2, width / 2, height / 2,
            -length / 2, width / 2, height / 2,
            -length / 2, -width / 2, height / 2,
            length / 2, -width / 2, height / 2
        }, {
            2, 1, 0,
            0, 3, 2,
            0, 4, 7,
            7, 3, 0,
            6, 5, 1,
            1, 2, 6,
            6, 7, 3,
            3, 2, 6,
            5, 4, 0,
            0, 1, 5,
            4, 5, 6,
            6, 7, 4
        });
    };

protected:
    float length;
    float width;
    float height;
};


class UIRectangle: public Model {
public:
    UIRectangle(vec2 pos, vec2 size): m_pos(pos), m_size(size), Model() {
        auto length = m_size[0], width = m_size[1];
        auto pos_x = m_pos[0], pos_y = m_pos[1];
        loadVertexCoord({
            pos_x + length / 2, pos_y + width / 2, 0,
            pos_x - length / 2, pos_y + width / 2, 0,
            pos_x - length / 2, pos_y - width / 2, 0,
            pos_x + length / 2, pos_y + width / 2, 0,
            pos_x - length / 2, pos_y - width / 2, 0,
            pos_x + length / 2, pos_y - width / 2, 0,
        }, {
            1, 1,
            0, 1,
            0, 0,
            1, 1,
            0, 0,
            1, 0,
        });
        setShader({"Package/shader/ui.vs", "package/shader/ui.fs"});
    };

protected:
    vec2 m_size;
    vec2 m_pos;
};


class UICharacter: public Model {
public:
    UICharacter(vec2 pos, int height): m_pos(pos), m_height(height), m_diff(1.0f/600.0f), Model() {
        setShader({"Package/shader/ui.vs", "package/shader/ui_text.fs"});
    }

    void setFontTexture(string font, char ch) {
        FontTexture * texture = new FontTexture;
        texture->bindTexture(0);
        texture->loadFont(font, ch, m_height);
    
        // replace old texture with new one
        if (m_textures.find(0) == m_textures.end()) {
            m_textures[0] = texture;
            auto size = texture->getFontRect() * m_diff;
            auto width = size[0], height = size[1];
            auto pos = m_pos + texture->getFontOffset() * m_diff;
            auto pos_x = pos[0], pos_y = pos[1];
            loadVertexCoord({
                pos_x + width, pos_y - height, 0,
                pos_x,         pos_y - height, 0,
                pos_x,         pos_y,          0,
                pos_x + width, pos_y - height, 0,
                pos_x,         pos_y,          0,
                pos_x + width, pos_y,          0,
            }, {
                1, 1,   // flip y texcoord since it is inverse of freetype lib..
                0, 1,
                0, 0,
                1, 1,
                0, 0,
                1, 0,
            });
        }
    }

    float getFontWidth() {
        return reinterpret_cast<FontTexture*>(m_textures[0])->getFontAdvance() * m_diff;
    }

protected:
    int m_height;
    vec2 m_pos;
    float m_diff;
};