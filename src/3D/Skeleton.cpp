#include "3D/Skeleton.hpp"
#include "utils/StringUtils.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "LogManager.hpp"

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
}

void Skeleton::update() {
    if (m_boneTree) {
        m_boneTree->update();
    }
}

void Skeleton::loadFromFile(string name) {
    fstream file;
    file.open(name, std::ios::in);
    string line;
    while (getline(file, line)) {
        line = string_lstrip(line, " ");
        line = string_lstrip(line, "\t");
        auto tokens = string_space_split(line);
        for (auto t: tokens)
            INFO("%s ", t.c_str()); 
        INFO("\n");
        if (tokens.empty()) {
            continue;
        }
        if (tokens[0] == "balljoint") {
            pushBone(tokens[1]);
        } else if (tokens[0][0] == '}') {
            popBone();
        } else if (tokens[0] == "offset") {
            m_boneTree->m_localPosition.x = stof(tokens[1]);
            m_boneTree->m_localPosition.y = stof(tokens[2]);
            m_boneTree->m_localPosition.z = stof(tokens[3]);
        } else if (tokens[0] == "boxmin") {
            m_boneTree->m_boxmin.x = stof(tokens[1]);
            m_boneTree->m_boxmin.y = stof(tokens[2]);
            m_boneTree->m_boxmin.z = stof(tokens[3]); 
        } else if (tokens[0] == "boxmax") {
            m_boneTree->m_boxmax.x = stof(tokens[1]);
            m_boneTree->m_boxmax.y = stof(tokens[2]);
            m_boneTree->m_boxmax.z = stof(tokens[3]);
        } else if (tokens[0] == "pose") {
            for (int i = 0; i < 3; i++) {
                m_boneTree->m_dofs[i].value = stof(tokens[i + 1]);
            }
        } else if (tokens[0] == "rotxlimit") {
            m_boneTree->m_dofs[0].min = stof(tokens[1]);
            m_boneTree->m_dofs[0].max = stof(tokens[2]);
        } else if (tokens[0] == "rotylimit") {
            m_boneTree->m_dofs[1].min = stof(tokens[1]);
            m_boneTree->m_dofs[1].max = stof(tokens[2]);
        } else if (tokens[0] == "rotzlimit") {
            m_boneTree->m_dofs[2].min = stof(tokens[1]);
            m_boneTree->m_dofs[2].max = stof(tokens[2]);
        } else {
            INFO("----------[%s]\n", tokens[0].c_str());
        }
    }
    file.close();
}

void Skeleton::pushBone(string name) {
    Bone * newBone = new Bone(name, m_boneTree);
    m_boneTree = newBone;
}

void Skeleton::popBone() {
    auto dofs = m_boneTree->m_dofs;
    m_boneTree->m_localMatrix = glm::eulerAngleZYX(dofs[2].getValue(), dofs[1].getValue(), dofs[0].getValue());
    auto boxSize = (m_boneTree->m_boxmax - m_boneTree->m_boxmin);
    m_boneTree->m_offset = (m_boneTree->m_boxmin + m_boneTree->m_boxmax) / 2.0f;

    INFO("%f %f %f\n", boxSize.x, boxSize.y, boxSize.z);
    m_boneTree->m_box = new Box(boxSize.x, boxSize.y, boxSize.z);
    m_boneTree->m_box->setPolygonMode(GL_LINE);
    m_boneTree->m_box->setShader({"package/shader/common_light_no_tex.vs", "package/shader/common_light_no_tex.fs"});
    
    if (m_boneTree->m_parent) {
        m_boneTree = m_boneTree->m_parent;
    }
}