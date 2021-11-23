#include "GLObject/Light.hpp"

LightMgr * LightMgr::s_instance = nullptr;

Light * LightMgr::createLight(LightType type) {
    Light * light = new Light;
    m_lights.push_back(light);
    return light;
}


vector<LightInfo> LightMgr::getLightInfo() {
    vector<LightInfo> lightInfo(10);
    for (auto i = 0; i < m_lights.size(); i++) {
        lightInfo[i] = m_lights[i]->getLightInfo();
    }
    return lightInfo;
}

LightMgr *LightMgr::instance() {
    if (s_instance == nullptr) {
        s_instance = new LightMgr;
    }
    return s_instance;
}
