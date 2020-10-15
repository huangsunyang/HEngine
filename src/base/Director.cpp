#include "base/Director.hpp"
#include "ui/Scene.hpp"
#include "LogManager.hpp"

Director * Director::m_instance = nullptr;


void Director::draw() {
    if (m_scene) {
        m_scene->draw();
    }
}

void Director::onTouchEvent(Touch * e) {
    if (m_scene) {
        m_scene->onTouchEvent(e);
        if (e->event == TouchEvent::END) {
            e->event = TouchEvent::CANCEl;
            m_scene->onTouchEvent(e);
        }
    }
}
