#include "3D/Skeleton.hpp"
#include "3D/Skin.hpp"
#include "utils/StringUtils.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "LogManager.hpp"
#include "Utils/Parser.hpp"

using std::fstream;
using std::string;
using std::getline;
using std::stof;
using Utils::string_lstrip;
using Utils::string_starts_with;
using Utils::string_space_split;

void Skeleton::draw() {
    if (m_boneTree) {
        m_boneTree->draw();
    }
    if (m_skin) {
        m_skin->draw();
    }
}

void Skeleton::update(float dt) {
    updateAnimation(dt);
    if (m_boneTree) {
        m_boneTree->update();
    }
    m_skin->update();
}

void Skeleton::updateAnimation(float dt) {
    if (m_curAnimation.empty()) {
        return;
    }

    if (m_animations.find(m_curAnimation) == m_animations.end()) {
        return;
    }

    m_curAnimationTime += dt;
    auto animation = m_animations.at(m_curAnimation);
    auto pose = animation->getPose(m_curAnimationTime);
    updatePose(pose);
}

void Skeleton::resetAnimation() {
    m_curAnimationTime = 0.0f;
}

void Skeleton::updatePose(std::shared_ptr<Pose> pose) {
    auto i = 3;
    for (auto bone: m_bones) {
        bone->updateDof(pose->m_dofs[i], pose->m_dofs[i+1], pose->m_dofs[i+2]);
        i += 3;
    }
}

void Skeleton::load(string skeleton_file, string skin_file) {
    loadFromFile(skeleton_file);
    if (!m_skin) m_skin = new Skin(this);
    m_skin->loadFromFile(skin_file);
}

void Skeleton::loadFromFile(string name) {
    fstream file(name, std::ios::in);
    Parser p(file);
    string line;
    while (getline(file, line)) {
        auto p = Parser(line);
        string token;
        p.parse(token);
        if (token == "balljoint") {
            pushBone(p.parse<string>());
        } else if (token[0] == '}') {
            popBone();
        } else if (token == "offset") {
            m_boneTree->m_localPosition.x = p.parse<float>();
            m_boneTree->m_localPosition.y = p.parse<float>();
            m_boneTree->m_localPosition.z = p.parse<float>();
        } else if (token == "boxmin") {
            m_boneTree->m_boxmin.x = p.parse<float>();
            m_boneTree->m_boxmin.y = p.parse<float>();
            m_boneTree->m_boxmin.z = p.parse<float>(); 
        } else if (token == "boxmax") {
            m_boneTree->m_boxmax.x = p.parse<float>();
            m_boneTree->m_boxmax.y = p.parse<float>();
            m_boneTree->m_boxmax.z = p.parse<float>();
        } else if (token == "pose") {
            for (int i = 0; i < 3; i++) {
                m_boneTree->m_dofs[i].value = p.parse<float>();
            }
        } else if (token == "rotxlimit") {
            m_boneTree->m_dofs[0].min = p.parse<float>();
            m_boneTree->m_dofs[0].max = p.parse<float>();
        } else if (token == "rotylimit") {
            m_boneTree->m_dofs[1].min = p.parse<float>();
            m_boneTree->m_dofs[1].max = p.parse<float>();
        } else if (token == "rotzlimit") {
            m_boneTree->m_dofs[2].min = p.parse<float>();
            m_boneTree->m_dofs[2].max = p.parse<float>();
        } else {
            INFO("----------[%s]\n", token.c_str());
        }
    }
    file.close();
}

void Skeleton::pushBone(string name) {
    Bone * newBone = new Bone(name, m_boneTree);
    m_boneTree = newBone;
    m_bones.push_back(newBone);
}

void Skeleton::popBone() {
    auto boxSize = (m_boneTree->m_boxmax - m_boneTree->m_boxmin);
    m_boneTree->m_offset = (m_boneTree->m_boxmin + m_boneTree->m_boxmax) / 2.0f;

    INFO("%f %f %f\n", boxSize.x, boxSize.y, boxSize.z);
    m_boneTree->m_box = new BoxWire(boxSize.x, boxSize.y, boxSize.z);
    m_boneTree->m_box->setPolygonMode(GL_LINE);
    m_boneTree->m_box->setShader({"package/shader/common_light_no_tex.vs", "package/shader/common_light_no_tex.fs"});
    
    if (m_boneTree->m_parent) {
        m_boneTree = m_boneTree->m_parent;
    }
}

void Skeleton::playAnimation(string file) {
    if (m_animations.find(file) == m_animations.end()) {
        loadAnimation(file);
    }
    m_curAnimation = file;
    m_curAnimationTime = 0.0f;
}

void Skeleton::loadAnimation(string file) {
    Animation * animation = new Animation;
    animation->loadFromFile(file);
    m_animations[file] = animation;
}