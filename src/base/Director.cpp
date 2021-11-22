#include "base/Director.hpp"
#include "ui/Scene.hpp"
#include "utils/LogManager.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "sb7/sb7color.h"

Director * Director::m_instance = nullptr;


Director * Director::instance() {
    if (!Director::m_instance) {
        Director::m_instance = new Director();
    }
    return m_instance;
}

CameraManager * Director::getCameraManager() {
    return CameraManager::getInstance();
}

void Director::draw2D() {
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

void Director::render() {
    auto cameras = getCameraManager()->getCameras();
    std::sort(cameras.begin(), cameras.end(), [](Camera * a, Camera * b) {
        if (a->getDepth() == b->getDepth()) {
            return a > b;
        } else {
            return a->getDepth() < b->getDepth();
        }
    });
    for (auto camera: cameras) {
        _renderOneCamera(camera);
    }

    glDisable(GL_DEPTH_TEST);
    glClearBufferfv(GL_COLOR, 0, sb7::color::White);
    glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
    glViewport(0, 0, m_screenSize.x, m_screenSize.y);
    for (auto camera: cameras) {
        camera->drawToScreen();
    }
}

void Director::_renderOneCamera(Camera * camera) {
    camera->draw();
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

void Director::setScreenSize(int w, int h) {
    m_screenSize = glm::vec2(w, h);
    m_aspect = (float) w / (float) h;

    for (auto camera: getCameraManager()->getCameras()) {
        camera->setAspect(m_aspect);
    }
}
