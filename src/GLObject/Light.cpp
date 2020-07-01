#include "GLObject/Light.hpp"

Light * LightMgr::createLight(LightType type) {
    Light * light = new Light;
    m_lights.push_back(light);
    return light;
}


vector<LightInfo> LightMgr::getLightInfo() {
    vector<LightInfo> lightInfo(10);
    for (int i = 0; i < m_lights.size(); i++) {
        lightInfo[i] = m_lights[i]->getLightInfo();
    }
    return lightInfo;
}