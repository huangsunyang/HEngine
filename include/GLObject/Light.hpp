#ifndef __LIGHT__
#define __LIGHT__

#include <map>
#include <vector>
#include "GLObject/Geometry.hpp"
#include "LogManager.hpp"
#include "glm/glm.hpp"
#include "sb7/vmath.h"

using std::map;
using std::vector;

enum class LightType {
    BASE,
    DIR,
    SPOT,
    POINT
};


struct LightInfo {
    glm::vec4 pos;
    glm::vec4 dir;
    glm::vec4 color;
};


class Light: public Box {
public:
    Light(): Box(2, 2, 2) {
        setShader({"package/shader/light.vs", "package/shader/common.fs"});
    }
    LightInfo getLightInfo() {
        auto pos = getTransform()->getPosition();
        auto forward = getTransform()->getForward();
        // DEBUG("lights info pos: %f %f %f\n", pos[0], pos[1], pos[2]);
        return {
            glm::vec4(pos, 0),
            glm::vec4(forward, 0),
            m_color
        };
    }

protected:
    glm::vec4 m_color;
};


class LightMgr {
public:
    LightMgr() {}
    ~LightMgr() {}

    Light * getLight(size_t index) {return m_lights[index];}
    vmath::uvec4 getLightNum() {return {m_lights.size(), m_lights.size(), m_lights.size(), m_lights.size()};}
    vector<LightInfo> getLightInfo();

    Light * createLight() {return createLight(LightType::BASE);}
    Light * createLight(LightType type);
    void addLight(Light *);
    void removeLight();

private:
    vector<Light *> m_lights;
    static const size_t m_maxLightNum = 10;
};

#endif