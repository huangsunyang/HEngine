#include "ObjLoader.hpp"


ObjLoader::ObjLoader(const char * name) {
    f.open(name, ios::in);
    parse();
    f.close();
}


void ObjLoader::parse() {
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


vector<float> ObjLoader::parse_float3_line(string line) {
    stringstream buffer(line);
    string header;
    float v1, v2, v3;
    buffer >> header >> v1 >> v2 >> v3;
    return vector<float>{v1, v2, v3};
}


void ObjLoader::parse_vertex_line(string line) {
    vector<float> point = parse_float3_line(line);
    vertex.insert(vertex.end(), point.begin(), point.end());
}


void ObjLoader::parse_normal_line(string line) {
    vector<float> point = parse_float3_line(line);
    normal.insert(normal.end(), point.begin(), point.end());
}


void ObjLoader::parse_surface_line(string line) {
    stringstream buffer(line);
    string header, each;
    vector<int> index;
    
    buffer >> header;

    while (getline(buffer, each)) {
        stringstream each_buffer(each);
        for (int i = 0; i < 3; i++) {
            int vi, ni, oi;
            char delim1, delim2;
            each_buffer >> vi >> delim1 >> delim2 >> ni;
            vertex_index.push_back(vi - 1);
            normal_index.push_back(ni - 1);
        }
    }
}


void test() {
    ObjLoader parser = ObjLoader("suzanne.obj");
    std::cout << parser.get_surface_num() << std::endl;
    std::cout << parser.get_vertex_num() << std::endl;
    system("pause");
}


int main1() {
    test();
    return 0;
}