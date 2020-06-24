#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "sb7/vmath.h"

class Transform {
public:
    Transform() {
        m_position = vmath::vec3(0, 0, 0);
        m_rotation = vmath::vec3(0, 0, 0);
        m_scale = vmath::vec3(1, 1, 1);
    }
    
    void setPosition(vmath::vec3 pos) {m_position = pos;}
    void setRotation(vmath::vec3 rot) {m_rotation = rot;}
    void setScale(vmath::vec3 scale) {m_rotation = scale;}
    
    void setPosition(float x, float y, float z) {m_position = vmath::vec3(x, y, z);}
    void setRotation(float x, float y, float z) {m_rotation = vmath::vec3(x, y, z);}
    void setScale(float x, float y, float z) {m_scale = vmath::vec3(x, y, z);}

    vmath::vec3 getPosition() {return m_position;}
    vmath::vec3 getRotation() {return m_rotation;}
    vmath::vec3 getScale() {return m_scale;}
    
    vmath::mat4 getMatrix() {
        return vmath::translate(m_position)
         * vmath::rotate(m_rotation[0], m_rotation[1], m_rotation[2]) * vmath::scale(m_scale);
    }

    vmath::vec3 getForward() {
        vmath::vec4 ret = vmath::rotate(m_rotation[0], m_rotation[1], m_rotation[2]) * vmath::vec4(0, 0, 1, 0);
        return vmath::vec3(ret[0], ret[1], ret[2]);
    }

protected:
    vmath::vec3 m_position;
    vmath::vec3 m_rotation;
    vmath::vec3 m_scale;
};

#endif