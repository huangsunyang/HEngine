#pragma once
#include "GLObject/Model.hpp"
#include "sb7/vmath.h"

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
    UIRectangle(vmath::vec2 pos, vmath::vec2 size): m_pos(pos), m_size(size) {
        auto length = m_size[0], width = m_size[1];
        auto pos_x = m_pos[0], pos_y = m_pos[1];
        loadVertexIndice({
            pos_x + length / 2, pos_y + width / 2, 0,
            pos_x - length / 2, pos_y + width / 2, 0,
            pos_x - length / 2, pos_y - width / 2, 0,
            pos_x + length / 2, pos_y - width / 2, 0,
        }, {
            0, 1, 2,
            0, 2, 3,
        });
        setShader({"Package/shader/ui.vs", "package/shader/common.fs"});
    };

protected:
    vmath::vec2 m_size;
    vmath::vec2 m_pos;
};