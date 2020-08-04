#pragma once
#include <algorithm>
#include "GLObject/Model.hpp"
#include "ui/Common.hpp"
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
    UICharacter() {
        setShader({"Package/shader/ui.vs", "package/shader/ui_text.fs"});
    }

    void setFontQuads(const vector<Quad> &quads) {
        vector<float> points;
        vector<float> texcoords;
        for (int i = 0; i < quads.size(); i++) {
            auto quad = quads[i];
            auto pos_x = quad.pos[0], pos_y = quad.pos[1];
            auto width = quad.size[0], height = quad.size[1];
            
            auto startxp = quad.startxp, startyp = quad.startyp;
            auto endxp = quad.endxp, endyp = quad.endyp;
            
            vector<float> temp{
                pos_x + width, pos_y - height, 0,
                pos_x,         pos_y - height, 0,
                pos_x,         pos_y,          0,
                pos_x + width, pos_y - height, 0,
                pos_x,         pos_y,          0,
                pos_x + width, pos_y,          0,
            };
            points.insert(points.end(), temp.begin(), temp.end());

            vector<float> temp1{
                endxp,      endyp,   // flip y texcoord since it is inverse of freetype lib..
                startxp,    endyp,
                startxp,    startyp,
                endxp,      endyp,
                startxp,    startyp,
                endxp,      startyp,
            };
            texcoords.insert(texcoords.end(), temp1.begin(), temp1.end());
        }
        loadVertexCoord(points, texcoords);
    }

protected:
    UIRectangle * rect;
};

class UIParticleDrawer: public UICharacter {
public:
    UIParticleDrawer() {
        setShader({"Package/shader/ui.vs", "package/shader/ui_texture.fs"});
    }
};
