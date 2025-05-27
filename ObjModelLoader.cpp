#include "ObjModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
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

    bool ObjModelLoader::Load(const std::string& obj_model_filepath) {
        std::string mtlFile, textureFile;

        std::cout << "Loading OBJ: " << obj_model_filepath << std::endl;

        if (!LoadOBJ(obj_model_filepath, mtlFile))
        {
            std::cerr << "Failed to load OBJ: " << obj_model_filepath << std::endl;
            return false;
        }

        // Descobrir o caminho base
        size_t lastSlash = obj_model_filepath.find_last_of("/\\");
        std::string basePath = (lastSlash == std::string::npos) ? "" : obj_model_filepath.substr(0, lastSlash + 1);

        std::cout << "Loading MTL: " << basePath + mtlFile << std::endl;

        if (!LoadMTL(basePath + mtlFile, textureFile))
        {
            std::cerr << "Failed to load MTL: " << mtlFile << std::endl;
            return false;
        }

        std::cout << "Loading texture: " << basePath + textureFile << std::endl;
        if (!LoadTexture(basePath + textureFile))
        {
            std::cerr << "Failed to load texture: " << textureFile << std::endl;
            return false;
        }

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

        // Error debug
        bool hasNormals = false;
        bool hasTextcoords = false;
        bool hasFaces = false;
        mtlFile.clear();

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
                        
            if (prefix == "mtllib") {
                if (!(iss >> mtlFile))
                    std::cerr << "Warning: MTL file declaration malformed in " << path << std::endl;
            }
            else if (prefix == "v") { // Vertex position
                glm::vec3 pos;
                if (iss >> pos.x >> pos.y >> pos.z)
                    temp_positions.push_back(pos);
            }
            else if (prefix == "vn") { // Vertex normal
                glm::vec3 norm;
                if (iss >> norm.x >> norm.y >> norm.z) {
                    temp_normals.push_back(norm);
                    hasNormals = true;
                }
            }
            else if (prefix == "vt") { // Vertex coordinate
                glm::vec2 tex;
                if (iss >> tex.x >> tex.y) {
                    temp_texcoords.push_back(tex);
                    hasTextcoords = true;
                }
            }
            else if (prefix == "f") { // Face
                hasFaces = true;
                std::string vertexStr;
                for (int i = 0; i < 3; ++i) {
                    if (!(iss >> vertexStr)) break;

                    std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                    std::istringstream viss(vertexStr);

                    unsigned int v, t = 0, n = 0;
                    viss >> v;

                    if (hasTextcoords) viss >> t;
                    if (hasNormals) viss >> n;

                    if (v > 0) {
                        vertexIndices.push_back(v - 1);
                        if (t > 0) texcoordIndices.push_back(t - 1);
                        if (n > 0) normalIndices.push_back(n - 1);
                    }
                }

                /*unsigned int v[3], t[3], n[3];
                char slash;
                for (int i = 0; i < 3; ++i) {
                    iss >> v[i] >> slash >> t[i] >> slash >> n[i];
                    vertexIndices.push_back(v[i]);
                    texcoordIndices.push_back(t[i]);
                    normalIndices.push_back(n[i]);
                }*/
            }
        }
        file.close();

        // Validation
        if (temp_positions.empty()) {
            std::cerr << "Error: No vertices found in " << path << std::endl;
            return false;
        }

        if (!hasFaces) {
            std::cerr << "Error: No faces found in " << path << std::endl;
            return false;
        }

        // Reorganizar os dados para OpenGL
        vertices.clear();
        indices.clear();

        try {
            for (size_t i = 0; i < vertexIndices.size(); ++i) {
                /*glm::vec3 pos = temp_positions[vertexIndices[i] - 1];
                glm::vec3 norm = temp_normals[normalIndices[i] - 1];
                glm::vec2 tex = temp_texcoords[texcoordIndices[i] - 1];*/

                // Position
                const auto& pos = temp_positions.at(vertexIndices.at(i));
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(pos.z);

                // Normal
                if (hasNormals && i < normalIndices.size()) {
                    const auto& norm = temp_normals.at(normalIndices.at(i));
                    vertices.push_back(norm.x);
                    vertices.push_back(norm.y);
                    vertices.push_back(norm.z);
                }
                else {
                    vertices.insert(vertices.end(), { 0, 0, 0 }); // Default normal
                }

                // Texture coordinate (option)
                if (hasTextcoords && i < texcoordIndices.size()) {
                    const auto& tex = temp_texcoords.at(texcoordIndices.at(i));
                    vertices.push_back(tex.x);
                    vertices.push_back(tex.y);
                }
                else {
                    vertices.insert(vertices.end(), { 0, 0 }); // Default UVs
                }

                indices.push_back(i);
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Error: Index out of range in " << path << ": " << e.what() << std::endl;
            return false;
        }

        std::cout << "Loaded OBJ: " << path
            << "(Verts: " << temp_positions.size()
            << ", Normals: " << temp_normals.size()
            << ", UVs: " << temp_texcoords.size()
            << ", Faces: " << vertexIndices.size() / 3 << ")" << std::endl;

        return true;
    }

    // Função simples para carregar .mtl (apenas map_Kd)
    bool ObjModelLoader::LoadMTL(const std::string& path, std::string& textureFile) {
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "Erro ao abrir arquivo MTL: " << path << std::endl;
            return false;
        }       

        textureFile.clear();
        bool foundTexture = false;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "map_Kd") {
                iss >> textureFile;
                foundTexture = true;
            }
        }
        file.close();

        if (!foundTexture) {
            std::cerr << "WARNING: No diffuse texture (map_Kd) found in " << path << std::endl;
        } else
            std::cout << "Found texture in MTL: " << textureFile << std::endl;

        return foundTexture;
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

} // namespace PoolGame3D