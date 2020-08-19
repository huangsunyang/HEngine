#include "3D/Skin.hpp"
#include "utils/StringUtils.hpp"
#include "utils/FileUtils.hpp"
#include <fstream>
#include "LogManager.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "3D/SkModel.hpp"

using Utils::string_space_split;
using Utils::string_lstrip;
using std::fstream;

void Skin::loadFromFile(string name) {
    m_filePath = name;
    fstream f(name, std::ios::in);
    string line;
    auto parsingState = -1;
    vector<glm::vec3> positions, normals;
    vector<glm::vec2> texcoords;
    auto has_texcoord = false;
    while (std::getline(f, line)) {
        line = string_lstrip(line, " ");
        line = string_lstrip(line, "\t");
        auto tokens = string_space_split(line);
        if (tokens[0] == "positions") {
            parsingState = 0;
        } else if (tokens[0] == "normals") {
            parsingState = 1;
        } else if (tokens[0] == "skinweights") {
            parsingState = 2;
        } else if (tokens[0] == "triangles") {
            parsingState = 3;
        } else if (tokens[0] == "bindings") {
            parsingState = 4;
        } else if (tokens[0] == "matrix") {
            pushMatrix();
        } else if (tokens[0] == "texcoords") {
            parsingState = 5;
            has_texcoord = true;
        } else if (tokens[0] == "material") {
            parsingState = 6;
        } else if (tokens.size() >= 2) {
            if (parsingState == 0) {
                positions.push_back(glm::vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2])));
            } else if (parsingState == 1) {
                normals.push_back(glm::vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2])));
            } else if (parsingState == 2) {
                auto num = stoi(tokens[0]);
                vector<Weight> weights;
                for (auto i = 0; i < num; i++) {
                    weights.push_back(Weight(stoi(tokens[i * 2 + 1]), stof(tokens[i * 2 + 2])));
                }
                m_weights.push_back(weights);
            } else if (parsingState == 3) {
                m_triangles.push_back(stoi(tokens[0]));
                m_triangles.push_back(stoi(tokens[1]));
                m_triangles.push_back(stoi(tokens[2]));
            } else if (parsingState == 4) {
                auto &curMatrix = m_bindingMatrixs.back();
                auto row = glm::vec4(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]), m_curRow == 3 ? 1.0: 0.0f);
                curMatrix[m_curRow++] = row;
            } else if (parsingState == 5) {
                texcoords.push_back(glm::vec2(stof(tokens[0]), stof(tokens[1])));
            } else if (parsingState == 6) {
                auto dir = Utils::file_dirname(m_filePath);
                auto tex_path = Utils::path_ensure_dir(tokens[1], dir);
                setTexture({tex_path});
            }
        }
    }
    for (int i = 0; i < positions.size(); i++) {
        m_positions.push_back(positions[i].x);
        m_positions.push_back(positions[i].y);
        m_positions.push_back(positions[i].z);
        m_positions.push_back(normals[i].x);
        m_positions.push_back(normals[i].y);
        m_positions.push_back(normals[i].z);
        if (has_texcoord) {
            m_positions.push_back(texcoords[i].x);
            m_positions.push_back(texcoords[i].y);
        }
    }
    INFO("------------------- %d %d %d %d", m_positions.size(), texcoords.size(), m_triangles.size(), m_bindingMatrixs.size());
    m_vertexInfo = new VertexInfo;
    m_vertexInfo->useIndice = true;
    m_vertexInfo->attrInfos = {
        {3, GL_FLOAT, 12},
        {3, GL_FLOAT, 12},
    };
    if (has_texcoord) {
        m_vertexInfo->attrInfos.push_back({2, GL_FLOAT, 8});
    }
    setShader({"package/shader/common_light.vs", "package/shader/common_light.fs"});

    initBuffers();
    f.close();
}

void Skin::pushMatrix() {
    m_curRow = 0;
    m_bindingMatrixs.push_back(glm::mat4(0.0f));
}

void Skin::update() {
    Skeleton * skeleton = m_model->getSkeleton();
    auto stride = 0;
    for (auto &m : m_vertexInfo->attrInfos) {
        stride += m.num;
    }
    for (int i = 0; i < m_weights.size(); i++) {
        glm::vec4 pos{m_positions[i * stride + 0], m_positions[stride * i + 1], m_positions[stride * i + 2], 1.0f};
        glm::vec4 normal{m_positions[i * stride + 3], m_positions[stride * i + 4], m_positions[stride * i + 5], 0.0f};
        glm::vec4 pos1{0.0, 0.0f, 0.0f, 0.0f};
        glm::vec4 normal1;
        for (auto weight: m_weights[i]) {
            auto bone = skeleton->getBone(weight.index);
            pos1 += bone->getWorldMatrix() * glm::inverse(m_bindingMatrixs[weight.index]) * pos * weight.weight;
            normal1 += bone->getWorldMatrix() * glm::inverse(m_bindingMatrixs[weight.index]) * normal * weight.weight;
        }
        normal1 = glm::normalize(normal1);
        m_positions[i * stride + 0] = pos1.x;
        m_positions[i * stride + 1] = pos1.y;
        m_positions[i * stride + 2] = pos1.z;
        m_positions[i * stride + 3] = normal1.x;
        m_positions[i * stride + 4] = normal1.y;
        m_positions[i * stride + 5] = normal1.z;
    }
    m_vbo->subData(getPoints());
}

void Skin::draw() {
    getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(glm::mat4(1.0f)));
    Drawable::draw();
}