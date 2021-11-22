#pragma once
#include <algorithm>
#include "ui/Common.hpp"
#include "glm/glm.hpp"
#include "GLObject/FontAtlas.hpp"
#include "ui/shape2d.hpp"


using glm::vec2;
using glm::vec3;
using glm::vec4;


class BoxWire: public HPolygon {
public:
    BoxWire(float length, float width, float height, float offsetx = 0, float offsety = 0, float offsetz = 0);

protected:
    float length;
    float width;
    float height;
};


class BoxSolid: public HPolygon {
public:
    BoxSolid(float length, float height, float width, float offsetx = 0, float offsety = 0, float offsetz = 0);

protected:
    float length;
    float width;
    float height;
};


class UIRectangle: public HPolygon {
public:
    UIRectangle(vec2 pos, vec2 size);

protected:
    vec2 m_size;
    vec2 m_pos;
};

class BatchDrawer : public HPolygon {
public:
    void setQuads(const vector<Quad>& quads);
};

class UICharacter: public BatchDrawer {
public:
    UICharacter(): BatchDrawer() {
        setShader({"Package/shader/ui.vs", "package/shader/ui_text.fs"});
    }

protected:
    UIRectangle * rect;
};

class UIParticleDrawer: public BatchDrawer {
public:
    UIParticleDrawer() {
        setShader({"Package/shader/ui.vs", "package/shader/ui_texture.fs"});
    }
};
