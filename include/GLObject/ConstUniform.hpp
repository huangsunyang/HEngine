#ifndef __CONST_UNIFORM__
#define __CONST_UNIFORM__

#include "GLObject/VertexBuffer.hpp"

/* Const Uniform is constant among all shaders,
 * but it will change during program executaion
 */

class ConstUniform {
public:
    static ConstUniform * instance();
    ~ConstUniform() {}

private:
    ConstUniform(): m_ubo(new VertexBuffer) {}

    static ConstUniform * m_instance;
    VertexBuffer * m_ubo;   //uniform buffer object
};


#endif