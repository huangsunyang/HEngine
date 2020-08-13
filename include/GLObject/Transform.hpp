#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class Transform {
public:
    Transform() {
        m_position = glm::vec3(0, 0, 0);
        m_rotation = glm::vec3(0, 0, 0);
        m_scale = glm::vec3(1, 1, 1);
    }
    
    void setPosition(glm::vec3 pos) {m_position = pos;}
    void setRotation(glm::vec3 rot) {m_rotation = rot;}
    void setScale(glm::vec3 scale) {m_rotation = scale;}
    
    void setPosition(float x, float y, float z) {m_position = glm::vec3(x, y, z);}
    void setRotation(float x, float y, float z) {m_rotation = glm::vec3(x, y, z);}
    void setScale(float x, float y, float z) {m_scale = glm::vec3(x, y, z);}

    glm::vec3 getPosition() {return m_position;}
    glm::vec3 getRotation() {return m_rotation;}
    glm::vec3 getScale() {return m_scale;}
    
    glm::mat4 getMatrix() {
        auto id = glm::mat4(1.0f);
        return glm::translate(id, m_position)
         * glm::eulerAngleXYZ(m_rotation[0], m_rotation[1], m_rotation[2]) * glm::scale(id, m_scale);
    }

    glm::vec3 getForward() {
        glm::vec4 ret = glm::eulerAngleXYZ(m_rotation[0], m_rotation[1], m_rotation[2]) * glm::vec4(0, 0, 1, 0);
        return glm::vec3(ret[0], ret[1], ret[2]);
    }

protected:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};

#endif