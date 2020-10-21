#include "base/Director.hpp"
#include "ui/Scene.hpp"
#include "LogManager.hpp"
#include "glm/gtc/type_ptr.hpp"

Director * Director::m_instance = nullptr;


void Director::draw() {
    if (m_scene) {
        m_scene->draw();
    }
}

void Director::draw3D() {
    for (auto model: Director::instance()->getObjects()) {
        auto m_matrix = model->getTransformMatrix();
        model->getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(m_matrix));
        model->draw();
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
