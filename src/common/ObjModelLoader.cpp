#include "ObjModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PoolGame3D {

    ObjModelLoader::ObjModelLoader() : VAO(0), VBO(0), EBO(0), textureID(0) {}
    ObjModelLoader::~ObjModelLoader() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &textureID);
    }

    bool ObjModelLoader::Load(const std::string& obj_model_filepath, bool ignoreMtl) {
        std::string mtlFile, textureFile;
        if (!LoadOBJ(obj_model_filepath, mtlFile)) return false;

        if (!ignoreMtl) {
            // Descobrir o caminho base
            size_t lastSlash = obj_model_filepath.find_last_of("/\\");
            std::string basePath = (lastSlash == std::string::npos) ? "" : obj_model_filepath.substr(0, lastSlash + 1);
            if (!LoadMTL(basePath + mtlFile, textureFile)) return false;
            if (!LoadTexture(basePath + textureFile)) return false;
        }
        return true;
    }

    void ObjModelLoader::Install() {
        std::cout << "[INFO] Modelo instalado: " << (vertices.size() / 8) << " vértices, " << indices.size() << " índices." << std::endl;
        // Para debug detalhado, descomente abaixo:
        // for (size_t i = 0; i < std::min((size_t)10, vertices.size() / 8); ++i) {
        //     std::cout << "Vértice " << i << ": ("
        //               << vertices[i*8+0] << ", "
        //               << vertices[i*8+1] << ", "
        //               << vertices[i*8+2] << ")\n";
        // }

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
        // std::cout << "Modelo instalado com sucesso!" << std::endl; // Removido para evitar repetição
    }

    void ObjModelLoader::Render(const glm::vec3& position, const glm::vec3& orientation, GLuint shaderProgram, const glm::mat4& modelIn, const glm::mat4& view, const glm::mat4& projection) {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        GLint texLoc = glGetUniformLocation(shaderProgram, "ballTexture");
        glUniform1i(texLoc, 0);

        // Matriz de modelo (posição e orientação)
        glm::mat4 model = glm::translate(modelIn, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, orientation.y, glm::vec3(0, 1, 0));
        model = glm::rotate(model, orientation.x, glm::vec3(1, 0, 0));
        model = glm::rotate(model, orientation.z, glm::vec3(0, 0, 1));
        model = glm::scale(model, glm::vec3(1.0f));

        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        if (indices.size() == 0) return;
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Função simples para carregar .obj (apenas triângulos, 1 material, sem grupos)
    bool ObjModelLoader::LoadOBJ(const std::string& path, std::string& mtlFile) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        std::vector<glm::vec3> temp_positions;
        std::vector<glm::vec3> temp_normals;
        std::vector<glm::vec2> temp_texcoords;
        std::vector<unsigned int> vertexIndices, normalIndices, texcoordIndices;

        std::string line;
        int faceCount = 0;
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
                std::vector<unsigned int> v, t, n;
                std::string vert;
                while (iss >> vert) {
                    unsigned int vi = 0, ti = 0, ni = 0;
                    sscanf(vert.c_str(), "%u/%u/%u", &vi, &ti, &ni);
                    v.push_back(vi);
                    t.push_back(ti);
                    n.push_back(ni);
                }
                // Se for triângulo
                if (v.size() == 3) {
                    for (int i = 0; i < 3; ++i) {
                        vertexIndices.push_back(v[i]);
                        texcoordIndices.push_back(t[i]);
                        normalIndices.push_back(n[i]);
                    }
                }
                // Se for quad, faz dois triângulos
                else if (v.size() == 4) {
                    // Triângulo 1: 0,1,2
                    for (int i : {0,1,2}) {
                        vertexIndices.push_back(v[i]);
                        texcoordIndices.push_back(t[i]);
                        normalIndices.push_back(n[i]);
                    }
                    // Triângulo 2: 0,2,3
                    for (int i : {0,2,3}) {
                        vertexIndices.push_back(v[i]);
                        texcoordIndices.push_back(t[i]);
                        normalIndices.push_back(n[i]);
                    }
                }
                // Se for polígono maior, pode ser ignorado ou tratado (não esperado para esfera)
                faceCount++;
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
        bool result = !vertices.empty() && !indices.empty();
        return result;
    }

    // Função simples para carregar .mtl (apenas map_Kd)
    bool ObjModelLoader::LoadMTL(const std::string& path, std::string& textureFile) {
        std::ifstream file(path);
        if (!file.is_open()) {
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
        // std::cout << "[DEBUG] Tentando carregar textura: " << texturePath << std::endl;
        int width, height, nrChannels;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "[ERRO] Falha ao carregar textura: " << texturePath << std::endl;
            return false;
        }
        std::cout << "[INFO] Textura carregada: " << texturePath << std::endl;
        // Para debug detalhado, descomente abaixo:
        // std::cout << "[DEBUG] Textura carregada com sucesso: " << texturePath << " (" << width << "x" << height << ", canais: " << nrChannels << ")" << std::endl;
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

    void ObjModelLoader::setMaterialAndTexture(const std::string& mtlPath, const std::string& texturePath) {
        std::string dummy;
        // Carrega o MTL apenas para manter compatibilidade, mas ignora o arquivo de textura do MTL
        LoadMTL(mtlPath, dummy);
        // Carrega a textura manualmente
        LoadTexture(texturePath);
    }

}