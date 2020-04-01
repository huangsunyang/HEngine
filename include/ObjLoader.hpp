#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::fstream;
using std::getline;
using std::ios;
using std::stringstream;
using std::vector;


class ObjLoader {
public:
    ObjLoader(const char * filename);
    int get_vertex_num() {return vertex.size() / 3;}
    int get_index_num() {return vertex_index.size();}
    int get_surface_num() {return vertex_index.size() / 3;}
    float * get_points() {return &vertex[0];}
    int * get_indices() {return &vertex_index[0];}

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
    vector<float> vertex;
    vector<float> normal;
    vector<int> vertex_index;
    vector<int> normal_index;
};

#endif