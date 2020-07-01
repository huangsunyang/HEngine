#include "pugixml.hpp"
#include "LogManager.hpp"
#include "GLObject/UniformBlock.hpp"


std::vector<UniformBlockData> UniformBlock::m_uniformBlockMembers = {
};


UniformBlock * UniformBlock::m_instance = nullptr;


UniformBlock::UniformBlock() {
    m_ubo = new VertexBuffer;
    size_t total_size = 0;
    pugi::xml_parse_result result;
    pugi::xml_document doc;
    result = doc.load_file("package/config/UniformBlock.xml");
    INFO("%s: %s\n", doc.name(), result.description());

    pugi::xml_node root = doc.child("root").child(getBlockName().c_str());
    for (auto block = root.first_child(); block; block = block.next_sibling()) {
        string name = string(block.attribute("name").value());
        size_t size = std::atoi(block.attribute("size").value());
        DEBUG("%d %d %s\n", total_size, size, name.c_str());
        
        m_uniformBlockMembers.push_back({name, size, total_size});
        total_size += size;
    }

    m_ubo->alloc(total_size, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, getBindingIndex(), m_ubo->getHandle());
}


UniformBlock * UniformBlock::instance() {
    if (!m_instance) {
        m_instance = new UniformBlock;
    }
    return m_instance;
}


// todo get uniform by blockName
// if uniform block is constant, then why should we have 2 const uniform blocks?
// if not, why should it be a singleton???
UniformBlock * UniformBlock::instance(string name) {
    return m_instance;
}


void UniformBlock::setUniformBlockMember(std::string name, void * data) {
    for (const auto& cu: m_uniformBlockMembers) {
        if (cu.name == name) {
            m_ubo->subData(cu.offset, cu.size, data);
        }
    }
}
