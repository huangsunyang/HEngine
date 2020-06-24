#include "ObjLoader.hpp"
#include "utils/StringUtils.hpp"
#include "utils/FileUtils.hpp"
#include "LogManager.hpp"


using namespace Utils;


// http://paulbourke.net/dataformats/obj/


ObjLoader::ObjLoader(const string &name) {
    m_vertexInfo = new VertexInfo;
    m_vertexInfo->useIndice = false;
    m_vertexInfo->attrInfos = vector<VertexAttrInfo> {
        {3, GL_FLOAT, 12},
        {3, GL_FLOAT, 12},
        {2, GL_FLOAT, 8},
    };

    fstream f;
    f.open(name.c_str(), ios::in);
    parse(f);
    f.close();
}


void ObjLoader::parse(fstream& f) {
    string line;
    while (getline(f, line)) {
        if (line.size() < 2) {
            continue;
        }

        stringstream lineBuffer(line);
        string head, s0, s1, s2;
        float f0, f1, f2;

        if (string_starts_with(line, "v ")) {
            lineBuffer >> head >> f0 >> f1 >> f2;
            vertex.push_back(f0); vertex.push_back(f1); vertex.push_back(f2);
        } else if (string_starts_with(line, "vn ")) {
            lineBuffer >> head >> f0 >> f1 >> f2;
            normal.push_back(f0); normal.push_back(f1); normal.push_back(f2);
        } else if (string_starts_with(line, "vt ")) {
            lineBuffer >> head >> f0 >> f1;
            tex.push_back(f0); tex.push_back(f1);
        } else if (string_starts_with(line, "f ")) {
            parse_surface_line(lineBuffer);
        } else if (string_starts_with(line, "mtllib ")) {
            lineBuffer >> head >> s0;
            // fixme: should use Path::res_folder to get res
            mtllib = new MtlLoader(path_ensure_dir(s0, "package/res"));
            INFO("%s\n", path_ensure_dir(s0, "package/res").c_str());
        } else if (string_starts_with(line, "usemtl ")) {
            lineBuffer >> head >> s0;
            material = mtllib->getMaterial(s0);
        }

        if (f.eof()) {
            break;
        }
    }
}


void ObjLoader::parse_surface_line(stringstream& buffer) {
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

            --vi; --ni; --ti;
            
            vertex_and_normal.push_back(vertex[vi * 3 + 0]);
            vertex_and_normal.push_back(vertex[vi * 3 + 1]);
            vertex_and_normal.push_back(vertex[vi * 3 + 2]);
            
            vertex_and_normal.push_back(normal[ni * 3 + 0]);
            vertex_and_normal.push_back(normal[ni * 3 + 1]);
            vertex_and_normal.push_back(normal[ni * 3 + 2]);
            
            vertex_and_normal.push_back(tex[ti * 2 + 0]);
            vertex_and_normal.push_back(tex[ti * 2 + 1]);
        }
    }
}

VertexInfo * ObjLoader::getVertexInfo() {
    return m_vertexInfo;
}
