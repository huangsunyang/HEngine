#include "ObjLoader.hpp"
// http://paulbourke.net/dataformats/obj/


ObjLoader::ObjLoader(const string &name) {
    m_vertexInfo = new VertexInfo;
    m_vertexInfo->useIndice = false;
    m_vertexInfo->attrInfos = vector<VertexAttrInfo> {
        {3, GL_FLOAT, 12},
        {3, GL_FLOAT, 12}
    };
    f.open(name.c_str(), ios::in);
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
            int vi, ti, ni;
            char delim1, delim2;
            each_buffer >> vi >> delim1;
            delim2 = each_buffer.peek();
            if (delim2 == '/') {
                // f v//vn v//vn v//vn
                each_buffer >> delim2 >> ni;
            } else {
                // f v/vt/vn v/vt/vn v/vt/vn
                each_buffer >> ti >> delim2 >> ni;
            }

            vertex_index.push_back(vi - 1);
            normal_index.push_back(ni - 1);
        }

        int cur_size = vertex_index.size();
        for (int i = 3; i > 0; i--) {
            vertex_and_normal.push_back(vertex[vertex_index[cur_size - i] * 3 + 0]);
            vertex_and_normal.push_back(vertex[vertex_index[cur_size - i] * 3 + 1]);
            vertex_and_normal.push_back(vertex[vertex_index[cur_size - i] * 3 + 2]);
            
            vertex_and_normal.push_back(normal[normal_index[cur_size - i] * 3 + 0]);
            vertex_and_normal.push_back(normal[normal_index[cur_size - i] * 3 + 1]);
            vertex_and_normal.push_back(normal[normal_index[cur_size - i] * 3 + 2]);
        }
    }
}

VertexInfo * ObjLoader::getVertexInfo() {
    return m_vertexInfo;
}
