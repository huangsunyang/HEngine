#include "ui/Widget.hpp"
#include "GLObject/Geometry.hpp"
#include "LogManager.hpp"
#include "ui/Touch.hpp"
#include "pugixml.hpp"
#include "glm/gtc/type_ptr.hpp"


Widget::Widget(string fileName) {
    m_drawer = new UIRectangle(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_children = vector<Widget *>{};

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    pugi::xml_node scene = doc.child("Scene");
    setName(scene.attribute("name").value());

    auto pos = scene.child("position");
    auto size = scene.child("size");
    auto color = scene.child("color");

    setPosition({pos.attribute("x").as_float(), pos.attribute("y").as_float()});
    setSize({size.attribute("width").as_float(), size.attribute("height").as_float()});
    setColor({color.attribute("r").as_float(), color.attribute("g").as_float(), color.attribute("b").as_float()});
}

void Widget::draw() {
    if (!m_visible) {
        return;
    }
    drawSelf();
    for (auto child: m_children) {
        child->draw();
    }
}

void Widget::drawSelf() {
    auto color = m_color;
    if (m_clickedDown) {
        color *= vec4{0.8f, 0.8f, 0.8f, 1.0f};
    }
    glm::mat4 matrix(1.0f);
    matrix[0][0] = m_size.x;
    matrix[1][1] = m_size.y;
    matrix[3][0] = m_pos.x;
    matrix[3][1] = m_pos.y;
    m_drawer->getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(matrix));
    m_drawer->getProgram()->setVec4Uniform("color", glm::value_ptr(color));
    m_drawer->draw();
}

bool Widget::onTouchEvent(Touch * e) {
    // first transmit to children
    bool processed = false;
    for (auto child: m_children) {
        bool processed = child->onTouchEvent(e);
        if (processed && child->isSwallowTouch()) {
            return processed;
        }
    }

    if (m_touchEnabled && _canReceiveTouch(e)) {
        INFO("%s %s\n", m_name.c_str(), enumToString(e->event).c_str());
        if(m_callback) {
            m_callback(this, e);
        }
        m_clickedDown = e->event == TouchEvent::BEGAN;
        return true;
    } else {
        return false;
    }
}


void Widget::_refreshWorldPosition() {
    if (!m_parent) {
        m_worldPos = m_pos;
    } else {
        m_worldPos = m_pos + m_parent->getWorldPosition();
    }
}


bool Widget::_canReceiveTouch(Touch * touch) {
    if (touch->event == TouchEvent::CANCEl) {
        return m_clickedDown;
    }
    return _inTouchArea(touch->pos);
}


bool Widget::_inTouchArea(glm::vec2 pos) {
    auto x_min = m_pos[0] - m_size[0] / 2, x_max = m_pos[0] + m_size[0] / 2;
    auto y_min = m_pos[1] - m_size[1] / 2, y_max = m_pos[1] + m_size[1] / 2;
    return pos[0] >= x_min && pos[0] <= x_max && pos[1] >= y_min && pos[1] <= y_max;
}