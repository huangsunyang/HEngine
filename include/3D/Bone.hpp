#pragma once

#include <vector>
#include <limits>
#include <string>
#include "glm/glm.hpp"
#include "GLObject/Geometry.hpp"

using std::vector;
using std::numeric_limits;
using std::string;

#ifdef max
    #undef max
#endif

#ifdef min
    #undef min
#endif

struct Dof {
    Dof(float v=0.0f):
        min(numeric_limits<float>::lowest()),
        max(numeric_limits<float>::max()),
        value(v) {}

    float getValue() {
        return glm::clamp(value, min, max);
    }

    float min;
    float max; 
    float value;
};  


class Bone {
public:
    friend class Skeleton;
    Bone(string name, Bone * parent):
        m_name(name),
        m_parent(parent),
        m_children(),
        m_localMatrix(),
        m_worldMatrix(),
        m_localPosition(glm::vec3(0, 0, 0)),
        m_offset(glm::vec3(0, 0, 0)),
        m_boxmin(glm::vec3(0.1f, 0.1f, 0.1f)),
        m_boxmax(glm::vec3(0.1f, 0.1f, 0.1f)),
        m_dofs({Dof(), Dof(), Dof()}), 
        m_box(nullptr) {
            if (m_parent) {
                m_parent->addChild(this);
            }
        }
    
    void addChild(Bone *); 
    void update();
    void updateSelf();
    void updateDof(float x, float y, float z);
    
    void draw();
    void drawSelf();

    glm::mat4 getWorldMatrix() {return m_worldMatrix;}

    string getName() {return m_name;}
    vector<Bone *> getChildren() { return m_children;}

protected:
    Bone * m_parent;
    string m_name;
    vector<Bone *> m_children;
    glm::mat4 m_localMatrix;
    glm::mat4 m_worldMatrix;
    glm::vec3 m_localPosition;
    glm::mat4 m_matrix;
    glm::vec3 m_offset;
    glm::vec3 m_boxmin;
    glm::vec3 m_boxmax;
    vector<Dof> m_dofs;

    Box * m_box;
};