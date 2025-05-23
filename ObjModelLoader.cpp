#include "ObjModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace PoolGame3D {

    ObjModelLoader::ObjModelLoader() : VAO(0), VBO(0), EBO(0), textureID(0), shaderProgram(0) {}
    ObjModelLoader::~ObjModelLoader() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &textureID);
    }

    bool ObjModelLoader::Load(const std::string& obj_model_filepath) {
        std::string mtlFile, textureFile;
        if (!LoadOBJ(obj_model_filepath, mtlFile)) return false;

        // Descobrir o caminho base
        size_t lastSlash = obj_model_filepath.find_last_of("/\\");
        std::string basePath = (lastSlash == std::string::npos) ? "" : obj_model_filepath.substr(0, lastSlash + 1);

        if (!LoadMTL(basePath + mtlFile, textureFile)) return false;
        if (!LoadTexture(basePath + textureFile)) return false;

        return true;
    }

    void ObjModelLoader::Install() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // pos(3), normal(3), texcoord(2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void ObjModelLoader::Render(const glm::vec3& position, const glm::vec3& orientation, GLuint shaderProgram, const glm::mat4& viewProj) {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Matriz de modelo (posição e orientação)
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, orientation.y, glm::vec3(0, 1, 0));
        model = glm::rotate(model, orientation.x, glm::vec3(1, 0, 0));
        model = glm::rotate(model, orientation.z, glm::vec3(0, 0, 1));
        glm::mat4 mvp = viewProj * model;

        GLuint mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Função simples para carregar .obj (apenas triangulos, 1 material, sem grupos)
    bool ObjModelLoader::LoadOBJ(const std::string& path, std::string& mtlFile) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir arquivo OBJ: " << path << std::endl;
            return false;
        }

        std::vector<glm::vec3> temp_positions;
        std::vector<glm::vec3> temp_normals;
        std::vector<glm::vec2> temp_texcoords;
        std::vector<unsigned int> vertexIndices, normalIndices, texcoordIndices;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "mtllib") {
                iss >> mtlFile;
            }
            else if (prefix == "v") {
                glm::vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                temp_positions.push_back(pos);
            }
            else if (prefix == "vn") {
                glm::vec3 norm;
                iss >> norm.x >> norm.y >> norm.z;
                temp_normals.push_back(norm);
            }
            else if (prefix == "vt") {
                glm::vec2 tex;
                iss >> tex.x >> tex.y;
                temp_texcoords.push_back(tex);
            }
            else if (prefix == "f") {
                unsigned int v[3], t[3], n[3];
                char slash;
                for (int i = 0; i < 3; ++i) {
                    iss >> v[i] >> slash >> t[i] >> slash >> n[i];
                    vertexIndices.push_back(v[i]);
                    texcoordIndices.push_back(t[i]);
                    normalIndices.push_back(n[i]);
                }
            }
        }
        file.close();

        // Reorganizar os dados para OpenGL
        vertices.clear();
        indices.clear();
        for (size_t i = 0; i < vertexIndices.size(); ++i) {
            glm::vec3 pos = temp_positions[vertexIndices[i] - 1];
            glm::vec3 norm = temp_normals[normalIndices[i] - 1];
            glm::vec2 tex = temp_texcoords[texcoordIndices[i] - 1];
            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(pos.z);
            vertices.push_back(norm.x);
            vertices.push_back(norm.y);
            vertices.push_back(norm.z);
            vertices.push_back(tex.x);
            vertices.push_back(tex.y);
            indices.push_back(i);
        }
        return true;
    }

    // Função simples para carregar .mtl (apenas map_Kd)
    bool ObjModelLoader::LoadMTL(const std::string& path, std::string& textureFile) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir arquivo MTL: " << path << std::endl;
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "map_Kd") {
                iss >> textureFile;
                break;
            }
        }
        file.close();
        return !textureFile.empty();
    }

    // Carrega textura usando stb_image
    bool ObjModelLoader::LoadTexture(const std::string& texturePath) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "Falha ao carregar textura: " << texturePath << std::endl;
            return false;
        }
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return true;
    }

    GLuint ObjModelLoader::GetShaderProgram() const {
        return shaderProgram;
    }

} // namespace PoolGame3D