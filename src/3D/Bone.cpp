#include "3D/Bone.hpp"
#include "LogManager.hpp"
#include "glm/gtc/type_ptr.hpp"

void Bone::addChild(Bone * bone) {
    m_children.push_back(bone);
}

void Bone::update() {
    updateSelf();
    for (auto bone: m_children) {
        bone->update();
    }
}

void Bone::updateSelf() {
    if (!m_parent) {
        m_worldMatrix = glm::translate(m_localMatrix, m_localPosition);
        m_matrix = glm::translate(m_localMatrix, m_localPosition + m_offset);
    } else {
        m_worldMatrix = glm::translate(m_parent->m_worldMatrix, m_localPosition) * m_localMatrix;
        m_matrix = glm::translate(m_parent->m_worldMatrix, m_localPosition) * glm::translate(m_localMatrix, m_offset); 
    }
}

void Bone::draw() {
    drawSelf();
    for (auto bone: m_children) {
        bone->draw();
    }
}

void Bone::drawSelf() {
    if (m_box) {
        m_box->getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(m_matrix));
        m_box->draw();
    }
}