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
    virtual size_t getPointNum() {return vertex.size() / 3;}
    virtual size_t getIndiceNum() {return vertex_index.size();}
    virtual GLfloat * getPoints() {return &vertex[0];}
    virtual GLuint * getIndices() {return &vertex_index[0];}
    float * get_vertex_and_normal() {return &vertex_and_normal[0];}

protected:
    void parse();
    void parse_vertex_line(string s);
    void parse_normal_line(string s);
    void parse_surface_line(string s);

private:
    vector<float> parse_float3_line(string s);

protected:
    string filename;
    fstream f;
    vector<GLfloat> vertex;
    vector<GLfloat> normal;
    vector<GLfloat> vertex_and_normal;
    vector<GLuint> vertex_index;
    vector<GLuint> normal_index;
};

#endif