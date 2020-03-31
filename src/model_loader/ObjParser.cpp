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


class ObjParser {
public:
    ObjParser(const char * filename);
    int get_vertex_num() {return vertex.size() / 3;}
    int get_surface_num() {return vertex_index.size() / 3;}

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


ObjParser::ObjParser(const char * name) {
    f.open(name, ios::in);
    parse();
    f.close();
}


void ObjParser::parse() {
    string line;
    while (getline(f, line)) {
        if (line.size() < 2) {
            continue;
        }

        if (line[0] == 'v' && line[1] == ' ') {
            parse_vertex_line(line);
        } else if (line[0] == 'v' && line[1] == 'n') {
            parse_normal_line(line);
        } else if (line[0] == 'f' && line[1] == ' ') {
            parse_surface_line(line);
        }

        if (f.eof()) {
            break;
        }
    }
}


vector<float> ObjParser::parse_float3_line(string line) {
    stringstream buffer(line);
    string header;
    float v1, v2, v3;
    buffer >> header >> v1 >> v2 >> v3;
    return vector<float>{v1, v2, v3};
}


void ObjParser::parse_vertex_line(string line) {
    vector<float> point = parse_float3_line(line);
    vertex.insert(vertex.end(), point.begin(), point.end());
}


void ObjParser::parse_normal_line(string line) {
    vector<float> point = parse_float3_line(line);
    normal.insert(normal.end(), point.begin(), point.end());
}


void ObjParser::parse_surface_line(string line) {
    stringstream buffer(line);
    string header, each;
    vector<int> index;
    
    buffer >> header;

    while (getline(buffer, each)) {
        stringstream each_buffer(each);

        int vi, ni, oi;
        string delim;
        each_buffer >> vi >> delim >> ni;
        vertex_index.push_back(vi);
        normal_index.push_back(ni);
        if (delim == "/") {
            each_buffer >> delim >> oi;
        }
    }
}


void test() {
    ObjParser parser = ObjParser("suzanne.obj");
    std::cout << parser.get_surface_num() << std::endl;
    std::cout << parser.get_vertex_num() << std::endl;
    system("pause");
}


int main() {
    test();
    return 0;
}