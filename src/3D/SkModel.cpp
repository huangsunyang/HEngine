#include "3D/SkModel.hpp"

void SkModel::load(string skeletonFile, string skinFile) {
    m_skeleton = new Skeleton();
    m_skeleton->loadFromFile(skeletonFile);
    m_skin = new Skin(this);
    m_skin->loadFromFile(skinFile);
}

void SkModel::draw() {
    // m_skeleton->draw();
    m_skin->setModelMatrix(m_skeleton->getBoneTree()->getWorldMatrix());
    m_skin->draw();
}

void SkModel::update() {
    m_skeleton->update();
    m_skin->update();
}