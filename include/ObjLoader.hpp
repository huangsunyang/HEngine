#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "GLObject/iMesh.hpp"

using std::string;
using std::fstream;
using std::getline;
using std::ios;
using std::stringstream;
using std::vector;


class ObjLoader: public iMesh {
public:
    ObjLoader(const string& filename);
    virtual size_t getPointNum() {return vertex_index.size();}
    virtual size_t getIndiceNum() {return vertex_index.size();}
    virtual GLfloat * getPoints() {return &vertex_and_normal[0];}
    virtual GLuint * getIndices() {return nullptr;}
    virtual VertexInfo * getVertexInfo();

protected:
    void parse();
    void parse_vertex_line(string s);
    void parse_normal_line(string s);
    void parse_tex_line(string s);
    void parse_surface_line(string s);

private:
    vector<float> parse_float3_line(string s);
    vector<float> parse_floatn(string, int);

protected:
    string filename;
    fstream f;

    VertexInfo * m_vertexInfo;
    vector<GLfloat> vertex;
    vector<GLfloat> normal;
    vector<GLfloat> tex;
    vector<GLfloat> vertex_and_normal;
    vector<GLuint> vertex_index;
    vector<GLuint> normal_index;
    vector<GLuint> tex_index;
};

#endif