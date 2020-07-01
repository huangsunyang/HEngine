#ifndef __CONST_UNIFORM__
#define __CONST_UNIFORM__

#include <map>
#include <string>
#include <vector>
#include "GLObject/VertexBuffer.hpp"

using std::map;
using std::string;
using std::vector;

struct UniformBlockMemberData {
    string name;
    size_t size;
    size_t offset;
};


/* Const Uniform is constant among all shaders,
 * but it will change during program executaion
 */
class UniformBlock {
public:
    static UniformBlock * instance();
    static UniformBlock * instance(string blockName);
    ~UniformBlock() {}

    void setUniformBlockMember(std::string name, void * data);
    GLuint getBindingIndex() {return 2;}
    string getBlockName() {return m_blockName;}

private:
    UniformBlock(string name);

    static UniformBlock * m_instance;

    VertexBuffer * m_ubo;   //uniform buffer object
    string m_blockName;
    map<string, UniformBlockMemberData> m_uniformBlockMembers;
};


#endif