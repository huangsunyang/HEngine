#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "GLObject/iMesh.hpp"
#include "MtlLoader.hpp"

using std::string;
using std::fstream;
using std::stringstream;
using std::getline;
using std::ios;
using std::stringstream;
using std::vector;


class ObjLoader: public iMesh {
public:
    ObjLoader(const string& filename);
    virtual ~ObjLoader();
    virtual size_t getPointNum() {return vertex_and_normal.size() / 8;}
    virtual size_t getIndiceNum() {return -1;}
    virtual GLfloat * getPoints() {return &vertex_and_normal[0];}
    virtual GLuint * getIndices() {return nullptr;}
    virtual VertexInfo * getVertexInfo();
    virtual vector<string> getTextures() {return {material->map_Kd};}

protected:
    void parse(fstream&);
    void parse_surface_line(stringstream&);

protected:
    string filename;

    VertexInfo * m_vertexInfo;
    vector<GLfloat> vertex;
    vector<GLfloat> normal;
    vector<GLfloat> tex;
    vector<GLfloat> vertex_and_normal;

    MtlLoader * mtllib;
    Material * material;
};

#endif