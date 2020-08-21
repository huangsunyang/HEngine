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
    auto has_texcoord = false;
    auto pos_i = 0, normal_i = 0, texcoord_i = 0, weight_i = 0;
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
                m_points.emplace_back();
                m_points[pos_i++].pos = glm::vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));
            } else if (parsingState == 1) {
                m_points[normal_i++].normal = glm::vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));
            } else if (parsingState == 2) {
                vector<Weight>& weights = m_points[weight_i++].weights;
                auto num = stoi(tokens[0]);
                for (auto i = 0; i < num; i++) {
                    weights.push_back(Weight(stoi(tokens[i * 2 + 1]), stof(tokens[i * 2 + 2])));
                }
            } else if (parsingState == 3) {
                m_indices.push_back(stoi(tokens[0]));
                m_indices.push_back(stoi(tokens[1]));
                m_indices.push_back(stoi(tokens[2]));
            } else if (parsingState == 4) {
                auto &curMatrix = m_bindingMatrixs.back();
                auto row = glm::vec4(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]), m_curRow == 3 ? 1.0: 0.0f);
                curMatrix[m_curRow++] = row;
            } else if (parsingState == 5) {
                m_points[texcoord_i++].texcoord = glm::vec2(stof(tokens[0]), stof(tokens[1]));
            } else if (parsingState == 6) {
                auto dir = Utils::file_dirname(m_filePath);
                auto tex_path = Utils::path_ensure_dir(tokens[1], dir);
                setTexture({tex_path});
            }
        }
    }
    for (int i = 0; i < m_points.size(); i++) {
        m_data.push_back(m_points[i].pos.x);
        m_data.push_back(m_points[i].pos.y);
        m_data.push_back(m_points[i].pos.z);
        m_data.push_back(m_points[i].normal.x);
        m_data.push_back(m_points[i].normal.y);
        m_data.push_back(m_points[i].normal.z);
        if (has_texcoord) {
            m_data.push_back(m_points[i].texcoord.x);
            m_data.push_back(m_points[i].texcoord.y);
        }
    }
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

void Skin::loadMorph(vector<string> files) {
    for (auto file: files) {
        loadMorph(file);
    }
}

void Skin::loadMorph(string file, float blend) {
    if (m_morphs.find(file) != m_morphs.end()) {
        return;
    }
    setMorphBlend(file, blend);
    auto &morph = m_morphs[file];
    fstream f(file, std::ios::in);
    string line;
    auto parsingState = -1;
    while (std::getline(f, line)) {
        line = string_lstrip(line, " ");
        line = string_lstrip(line, "\t");
        auto tokens = string_space_split(line);
        if (tokens[0] == "positions") {
            parsingState = 0;
        } else if (tokens[0] == "normals") {
            parsingState = 1;
        } else if (tokens.size() >= 2) {
            if (parsingState == 0) {
                morph[stoi(tokens[0])].pos = glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
            } else if (parsingState == 1) {
                morph[stoi(tokens[0])].normal = glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
            }
        }
    }
}

void Skin::pushMatrix() {
    m_curRow = 0;
    m_bindingMatrixs.push_back(glm::mat4(0.0f));
}

void Skin::update() {
    Skeleton * skeleton = m_model->getSkeleton();
    auto stride = getPointSize() / 4;
    for (int i = 0; i < m_points.size(); i++) {
        glm::vec4 pos = getMorphPosition(i);
        glm::vec4 normal = glm::vec4(m_points[i].normal, 0.0f);
        glm::vec4 pos1{ 0.0f, 0.0f, 0.0f, 0.0f }, normal1{ 0.0f, 0.0f, 0.0f, 0.0f };
        for (auto weight: m_points[i].weights) {
            auto bone = skeleton->getBone(weight.index);
            pos1 += bone->getWorldMatrix() * glm::inverse(m_bindingMatrixs[weight.index]) * pos * weight.weight;
            normal1 += bone->getWorldMatrix() * glm::inverse(m_bindingMatrixs[weight.index]) * normal * weight.weight;
        }
        normal1 = glm::normalize(normal1);
        m_data[i * stride + 0] = pos1.x;
        m_data[i * stride + 1] = pos1.y;
        m_data[i * stride + 2] = pos1.z;
        m_data[i * stride + 3] = normal1.x;
        m_data[i * stride + 4] = normal1.y;
        m_data[i * stride + 5] = normal1.z;
    }
    m_vbo->subData(getPoints());
}

glm::vec4 Skin::getMorphPosition(int i) {
    glm::vec3 pos = m_points[i].pos;
    for (const auto& pair: m_morphs) {
        const auto& morph = pair.second;
        auto blend = m_morphBlend[pair.first];
        if (morph.find(i) != morph.end()) {
            pos += morph.at(i).pos * blend;
        }
    }
    return glm::vec4(pos, 1.0f);
}

glm::vec4 Skin::getMorphNormal(int i) {
    glm::vec3 normal = m_points[i].normal;
    for (const auto& pair: m_morphs) {
        const auto& morph = pair.second;
        auto blend = m_morphBlend[pair.first];
        if (morph.find(i) != morph.end()) {
            normal += morph.at(i).normal * blend;
        }
    }
    return glm::vec4(glm::normalize(normal), 0.0f);
}

void Skin::draw() {
    getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(glm::mat4(1.0f)));
    Drawable::draw();
}