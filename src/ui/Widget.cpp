#include "ui/Widget.hpp"
#include "GLObject/Geometry.hpp"
#include "LogManager.hpp"

void Widget::draw() {
    if (!m_visible) {
        return;
    }

    if (!m_drawer) {
        m_drawer = new UIRectangle(m_pos, m_size);
    }
    m_drawer->getProgram()->setVec4Uniform("color", m_color);
    m_drawer->draw();
    
    for (auto child: m_children) {
        child->draw();
    }
}


void Widget::onTouchEvent(Touch * e) {
    if (!m_callback || !m_touchEnabled) return;
    
    if (_inTouchArea(e->pos)) {
        m_callback(this, e);
    } else {
        INFO("not in area %f %f %f %f\n", e->pos[0], e->pos[1], m_pos[0], m_pos[1]);
    }

    // transmit to children
    if (!m_swallowTouch) {
        for (auto child: m_children) {
            child->onTouchEvent(e);
        }
    }
}


void Widget::_refreshWorldPosition() {
    if (!m_parent) {
        m_worldPos = m_pos;
    } else {
        m_worldPos = m_pos + m_parent->getWorldPosition();
    }
}


bool Widget::_inTouchArea(vec2 pos) {
    auto x_min = m_pos[0] - m_size[0] / 2, x_max = m_pos[0] + m_size[0] / 2;
    auto y_min = m_pos[1] - m_size[1] / 2, y_max = m_pos[1] + m_size[1] / 2;
    INFO("%f %f %f %f\n", x_min, x_max, y_min, y_max);
    return pos[0] >= x_min && pos[0] <= x_max && pos[1] >= y_min && pos[1] <= y_max;
}