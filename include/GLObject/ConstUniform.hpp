#ifndef __CONST_UNIFORM__
#define __CONST_UNIFORM__

#include <map>
#include <string>
#include <vector>
#include "GLObject/VertexBuffer.hpp"


struct ConstUniformData {
    std::string name;
    size_t size;
    size_t offset;
};


/* Const Uniform is constant among all shaders,
 * but it will change during program executaion
 */
class ConstUniform {
public:
    static ConstUniform * instance();
    ~ConstUniform() {}

    void setConstUniform(std::string name, void * data);
    GLuint getUBOHandle() {return 2;}

private:
    ConstUniform();

    static ConstUniform * m_instance;
    static std::vector<ConstUniformData> m_constUniformVec;

    VertexBuffer * m_ubo;   //uniform buffer object
};


#endif