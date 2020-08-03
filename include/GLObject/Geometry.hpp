#pragma once
#include "GLObject/Model.hpp"
#include "sb7/vmath.h"
#include "GLObject/FontAtlas.hpp"


using vmath::vec2;
using vmath::vec3;
using vmath::vec4;


class Box: public Model {
public:
    Box(float length, float width, float height): Model() {
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
        setShader({"Package/shader/ui.vs", "package/shader/ui_text.fs"});
    };

protected:
    vec2 m_size;
    vec2 m_pos;
};


class UICharacter: public Model {
public:
    UICharacter(vec2 pos, vec2 size): m_pos(pos), m_size(size), m_diff(1.0f/600.0f), Model() {
        setShader({"Package/shader/ui.vs", "package/shader/ui_text.fs"});
    }

    void setFontRect(Rect rect) {
        auto pos_x = m_pos[0], pos_y = m_pos[1];
        auto width = m_size[0], height = m_size[1];
        auto startxp = rect.startxp, startyp = rect.startyp;
        auto endxp = rect.endxp, endyp = rect.endyp;
        loadVertexCoord({
            pos_x + width, pos_y - height, 0,
            pos_x,         pos_y - height, 0,
            pos_x,         pos_y,          0,
            pos_x + width, pos_y - height, 0,
            pos_x,         pos_y,          0,
            pos_x + width, pos_y,          0,
        }, {
            endxp,      endyp,   // flip y texcoord since it is inverse of freetype lib..
            startxp,    endyp,
            startxp,    startyp,
            endxp,      endyp,
            startxp,    startyp,
            endxp,      startyp,
        });
    }

protected:
    vec2 m_pos;
    vec2 m_size;
    float m_diff;
    UIRectangle * rect;
};