#include "pugixml.hpp"
#include "LogManager.hpp"
#include "GLObject/UniformBlock.hpp"


UniformBlock * UniformBlock::m_instance = nullptr;


UniformBlock::UniformBlock(string name) {
    m_ubo = new VertexBuffer;
    m_blockName = name;
    DEBUG("new uniform block: %s\n", name.c_str());

    size_t total_size = 0;
    pugi::xml_parse_result result;
    pugi::xml_document doc;
    result = doc.load_file("package/config/UniformBlock.xml");
    DEBUG("%s: %s\n", "package/config/UniformBlock.xml", result.description());

    pugi::xml_node root = doc.child("root").child(name.c_str());
    for (auto block = root.first_child(); block; block = block.next_sibling()) {
        string name = string(block.attribute("name").value());
        size_t size = std::atoi(block.attribute("size").value());
        DEBUG("%d %d %s\n", total_size, size, name.c_str());
        
        m_uniformBlockMembers[name] = {name, size, total_size};
        total_size += size;
    }

    m_ubo->alloc(total_size, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, getBindingIndex(), m_ubo->getHandle());
}


UniformBlock * UniformBlock::instance() {
    if (!m_instance) {
        m_instance = new UniformBlock("ConstantBlock");
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
    if (m_uniformBlockMembers.find(name) == m_uniformBlockMembers.end()) {
        return;
    }
    auto& cu = m_uniformBlockMembers[name];
    m_ubo->subData(cu.offset, cu.size, data);
}
