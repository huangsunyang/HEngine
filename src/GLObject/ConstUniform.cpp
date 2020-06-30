#include "LogManager.hpp"
#include "GLObject/ConstUniform.hpp"


std::vector<ConstUniformData> ConstUniform::m_constUniformVec = {
    {"view_matrix", 64, 0},
    {"proj_matrix", 64, 0},
};


ConstUniform * ConstUniform::m_instance = nullptr;


ConstUniform::ConstUniform() {
    m_ubo = new VertexBuffer;
    size_t total_size = 0;
    for (auto& data: m_constUniformVec) {
        DEBUG("%d %d %d %s\n", total_size, data.offset, data.size, data.name.c_str());
        data.offset = total_size;
        total_size += data.size;
    }
    m_ubo->alloc(total_size, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, getUBOHandle(), m_ubo->getHandle());
}


ConstUniform * ConstUniform::instance() {
    if (!m_instance) {
        m_instance = new ConstUniform;
    }
    return m_instance;
}


void ConstUniform::setConstUniform(std::string name, void * data) {
    for (const auto& cu: m_constUniformVec) {
        if (cu.name == name) {
            m_ubo->subData(cu.offset, cu.size, data);
        }
    }
}
