#include "GLObject/Geometry.hpp"

BoxWire::BoxWire(float length, float width, float height, float offsetx, float offsety, float offsetz) : HPolygon() {
    // from bottom to up, from first area to fourth area
    loadVertexCoord({
        offsetx + length / 2, offsety + width / 2, offsetz - height / 2,
        offsetx - length / 2, offsety + width / 2, offsetz - height / 2,
        offsetx - length / 2, offsety - width / 2, offsetz - height / 2,
        offsetx + length / 2, offsety - width / 2, offsetz - height / 2,
        offsetx + length / 2, offsety + width / 2, offsetz + height / 2,
        offsetx - length / 2, offsety + width / 2, offsetz + height / 2,
        offsetx - length / 2, offsety - width / 2, offsetz + height / 2,
        offsetx + length / 2, offsety - width / 2, offsetz + height / 2
        }, {

        }, {
            2, 1, 1, 0,
            0, 3, 3, 2,
            0, 4, 4, 7,
            7, 3, 3, 0,
            6, 5, 5, 1,
            1, 2, 2, 6,
            6, 7, 7, 3,
            3, 2, 2, 6,
            5, 4, 4, 0,
            0, 1, 1, 5,
            4, 5, 5, 6,
            6, 7, 7, 4
        });
    setDrawMode(GL_LINES);
};

BoxSolid::BoxSolid(float length, float height, float width, float offsetx, float offsety, float offsetz) : HPolygon() {
    // from bottom to up, from first area to fourth area
    loadVertexCoord({
        offsetx + length / 2, offsety - height / 2, offsetz + width / 2,
        offsetx - length / 2, offsety - height / 2, offsetz + width / 2,
        offsetx - length / 2, offsety - height / 2, offsetz - width / 2,
        offsetx + length / 2, offsety - height / 2, offsetz - width / 2,
        offsetx + length / 2, offsety + height / 2, offsetz + width / 2,
        offsetx - length / 2, offsety + height / 2, offsetz + width / 2,
        offsetx - length / 2, offsety + height / 2, offsetz - width / 2,
        offsetx + length / 2, offsety + height / 2, offsetz - width / 2,
        offsetx + length / 2, offsety + height / 2, offsetz - width / 2,
        offsetx - length / 2, offsety + height / 2, offsetz - width / 2,
        offsetx - length / 2, offsety - height / 2, offsetz + width / 2,
        offsetx + length / 2, offsety - height / 2, offsetz + width / 2,
        }, {
            1, 1,
            0, 1,
            0, 0,
            1, 0,
            0, 1,
            1, 1,
            1, 0,
            0, 0,
            1, 1,
            0, 1,
            1, 0,
            0, 0,
        }, {
            2, 1, 0,  // bottom
            0, 3, 2,  // bottom
            4, 5, 6,  // top
            6, 7, 4,  // top
            0, 4, 7,  // right
            7, 3, 0,  // right
            6, 5, 1,  // left
            1, 2, 6,  // left
            9, 8, 3,  // front
            3, 2, 9,  // front
            4, 5, 10,
            10, 11, 4,
        });
};

UIRectangle::UIRectangle(vec2 pos, vec2 size) : m_pos(pos), m_size(size), HPolygon() {
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
        }, {});
    setShader({ "Package/shader/ui.vs", "package/shader/ui.fs" });
};

void BatchDrawer::setQuads(const vector<Quad>& quads) {
    vector<float> points;
    vector<float> texcoords;
    for (auto i = 0; i < quads.size(); i++) {
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
    loadVertexCoord(points, texcoords, {});
}

