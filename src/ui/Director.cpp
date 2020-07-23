#include "ui/Director.hpp"
#include "ui/Scene.hpp"

Director * Director::m_instance = nullptr;


void Director::draw() {
    if (m_scene) {
        m_scene->draw();
    }
}

void Director::onTouchEvent(Touch * e) {
    if (m_scene) {
        m_scene->onTouchEvent(e);
    }
}