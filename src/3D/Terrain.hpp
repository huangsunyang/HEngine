#include "GLObject/Drawable.hpp"
#include "shape2d.hpp"
#include "GLFW/glfw3.h"

class Terrain: public HPolygon {
public:
    Terrain() {
        loadVertexCoord({
            -0.5, 0, +0.5,
            -0.5, 0, -0.5,
            +0.5, 0, +0.5,
            +0.5, 0, -0.5,
        }, {}, {});
        setShader({
            "Package/shader/terrain/terrain.vs",
            "Package/shader/terrain/terrain.tesc",
            "Package/shader/terrain/terrain.tese",
            "Package/shader/terrain/terrain.fs",
        });
        auto len = 16;
        getProgram()->setIntUniform("len", len);
        getProgram()->setFloatUniform("dmap_depth", m_height);
        getProgram()->setFloatUniform("diff", m_diff);

        setTexture(0, "package/res/terrain_height_map.jpg");

        glPatchParameteri(GL_PATCH_VERTICES, 4);
        setDrawMode(GL_PATCHES);
        setInstanceCount(len * len);
    }

    void addWidth(float x) {
        INFO("add_width\n");
        m_verteces[0] -= x;
        m_verteces[3] -= x;
        m_verteces[6] += x;
        m_verteces[9] += x;
        m_vbo->subData(getPoints());
    }

    void addHeight(float x) {
        m_height += x;
        getProgram()->setFloatUniform("dmap_depth", m_height);
    }

    void addDiff(float x) {
        if (x > 0) m_diff *= x;
        else m_diff /= -x;
        getProgram()->setFloatUniform("diff", m_diff);
    }

    float m_height = 10.0f;
    float m_diff = 0.001f;
};
