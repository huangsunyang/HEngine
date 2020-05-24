#ifndef __MESH__
#define __MESH__

#include <vector>
#include "GL/gl3w.h"

using namespace std;


struct VertexAttrInfo {
    int num;
    GLenum type;   // type is an enum，GL_FLOAT but not GLfloat
    size_t size;   // equal to num * sizeof(Type)
};


struct VertexInfo {
    bool useIndice;                     // 是否使用索引绘制，即使用drawElements而不是drawArray
    vector<VertexAttrInfo> attrInfos;   // 每个属性的信息
};


class iMesh {
public:
    virtual size_t getPointNum() {return 0;} 
    virtual size_t getIndiceNum() {return 0;}
    virtual GLfloat * getPoints() {return nullptr;}      // point_num * 3
    virtual GLfloat * getTexcoord() {return nullptr;}    // point_num * 2
    virtual GLuint * getIndices() {return nullptr;}      // indexnum
    virtual VertexInfo * getVertexInfo() {return nullptr;}
};

#endif