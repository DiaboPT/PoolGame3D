#include "ObjModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "LoadShaders.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PoolGame3D {

    ObjModelLoader::ObjModelLoader() {}
    ObjModelLoader::~ObjModelLoader() {}

    void ObjModelLoader::Load(const std::string obj_model_filepath, GLuint sPos, GLuint sNormal, GLuint sTextCoord, GLuint textureBind, GLuint shader, int counter) {
        std::string mtlFile, textureFile;
        this->shaderProgram = shader;
        this->sPos = sPos;
        this->sNormal = sNormal;
        this->sTextCoord = sTextCoord;
        this->textureBind = textureBind;
		this->texCounter = counter;

        std::cout << "Loading OBJ: " << obj_model_filepath << std::endl;

        LoadOBJ(obj_model_filepath);}

    void ObjModelLoader::Install() {

        GLfloat lPositions[8064 * 3 * 3];
        GLfloat lNormals[8064 * 3 * 3];
        GLfloat lTextureCoords[8064 * 2 * 3];

        std::cout << "Number of positions : " << vertexIndices.size() << std::endl;
        std::cout << "Number of normals : " << normalIndices.size() << std::endl;
        std::cout << "Number of texCoords : " << texcoordIndices.size() << std::endl;
    
        for (int i = 0; i < vertexIndices.size(); ++i) {
            // Vertex
            lPositions[i * 3] = vertexIndices[i].x;
            lPositions[i * 3 + 1] = vertexIndices[i].y;
            lPositions[i * 3 + 2] = vertexIndices[i].z;

            // Normals
            lNormals[i * 3] = normalIndices[i].x;
            lNormals[i * 3 + 1] = normalIndices[i].y;
            lNormals[i * 3 + 2] = normalIndices[i].z;

            // Texture Coords
            lTextureCoords[i * 2] = texcoordIndices[i].x;
            lTextureCoords[i * 2 + 1] = texcoordIndices[i].y;
        }

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(3, VBO);
                

        
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL error after VAO creation: " << err << std::endl;
        }

        for (int i = 0; i < 3; ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

            if (i == 0) glBufferStorage(GL_ARRAY_BUFFER, sizeof(lPositions), lPositions, 0);

            if (i == 1) glBufferStorage(GL_ARRAY_BUFFER, sizeof(lNormals), lNormals, 0);

            if (i == 2) glBufferStorage(GL_ARRAY_BUFFER, sizeof(lTextureCoords), lTextureCoords, 0);
        }


        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(sPos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glVertexAttribPointer(sNormal, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glVertexAttribPointer(sTextCoord, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(sPos);
        glEnableVertexAttribArray(sNormal);
        glEnableVertexAttribArray(sTextCoord);

        glProgramUniform1i(shaderProgram, textureBind, 0);
    }

    void ObjModelLoader::Render(glm::vec3 position, glm::vec3 orientation, glm::mat4 modelMatrix) {
        using namespace glm;

        // Matriz de modelo (posi��o e orienta��o)
        mat4 tempModel = modelMatrix;
        tempModel = translate(tempModel, position);

        tempModel = rotate(tempModel, radians(orientation.x), vec3(1, 0, 0));
        tempModel = rotate(tempModel, radians(orientation.y), vec3(0, 1, 0));
        tempModel = rotate(tempModel, radians(orientation.z), vec3(0, 0, 1));

        GLint modelId = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "Model");
        glProgramUniformMatrix4fv(shaderProgram, modelId, 1, GL_FALSE, value_ptr(tempModel));

        mat4 modelView = cam::Camera::GetInstance()->view * tempModel;
        GLint modelViewId = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "ModelView");
        glProgramUniformMatrix4fv(shaderProgram, modelViewId, 1, GL_FALSE, value_ptr(modelView));

        mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelView));
        GLint normalMatrixId = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "NormalMatrix");
        glProgramUniformMatrix4fv(shaderProgram, normalMatrixId, 1, GL_FALSE, value_ptr(normalMatrix));

        GLint viewId = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "View");
        glProgramUniformMatrix4fv(shaderProgram, viewId, 1, GL_FALSE, value_ptr(cam::Camera::GetInstance()->view));

        GLint projectionId = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "Projection");
        glProgramUniformMatrix4fv(shaderProgram, projectionId, 1, GL_FALSE, value_ptr(cam::Camera::GetInstance()->projection));

        glBindTexture(GL_TEXTURE_2D, textureID + 1);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, vertexIndices.size());
    }

    // Fun��o simples para carregar .obj (apenas triangulos, 1 material, sem grupos)
    void ObjModelLoader::LoadOBJ(const std::string& path) {
        using namespace std;

        ifstream file(path);
        
        if (!file.is_open()) {
            cerr << "Erro ao abrir arquivo OBJ: " << path << endl;
        }        

        vector<glm::vec3> temp_positions;
        vector<glm::vec3> temp_normals;
        vector<glm::vec2> temp_texcoords;       

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            istringstream iss(line);
            string prefix;
            iss >> prefix;
                        
            if (prefix == "mtllib") {
                string mtlFile;
                iss >> mtlFile;
                ObjModelLoader::LoadMTL(mtlFile);
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
                }
            }
            else if (prefix == "vt") { // Vertex coordinate
                glm::vec2 tex;
                if (iss >> tex.x >> tex.y) {
                    temp_texcoords.push_back(tex);
                }
            }
            else if (prefix == "f") { // Face
                string vertexStr;
                for (int i = 0; i < 3; ++i)
                {
                    string vertexData;
                    iss >> vertexData;

                    istringstream viss(vertexData);
                    string index;

                    getline(viss, index, '/');
                    int positionIndex = stoi(index) - 1;

                    getline(viss, index, '/');
                    int texcoordIndex = stoi(index) - 1;

                    getline(viss, index, '/');
                    int normalIndex = stoi(index) - 1;

                    vertexIndices.push_back(temp_positions[positionIndex]);
                    normalIndices.push_back(temp_normals[normalIndex]);
                    texcoordIndices.push_back(temp_texcoords[texcoordIndex]);
                }
            }
        }

        std::cout << temp_positions.size() << endl;
        std::cout << temp_normals.size() << endl;
        std::cout << temp_texcoords.size() << endl;

        std::cout << vertexIndices.size() << endl;
        std::cout << normalIndices.size() << endl;
        std::cout << texcoordIndices.size() << endl;

        file.close();
    }

    // Fun��o simples para carregar .mtl (apenas map_Kd)
    void ObjModelLoader::LoadMTL(const std::string& materialFile) {
        std::ifstream file("PoolBalls/" + materialFile);

        std::cout << "lOADING MTL: " << materialFile << std::endl << std::endl;            
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir arquivo MTL: " << std::endl;            
        }               

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "Ka")
            {
                iss >> ka.x >> ka.y >> ka.z;
            }
            else if (prefix == "Kd")
            {
                iss >> kd.x >> kd.y >> kd.z;
            }
            else if (prefix == "Ks")
            {
                iss >> ks.x >> ks.y >> ks.z;
            }
            else if (prefix == "Ns")
            {
                iss >> ns;
            }
            else if (prefix == "map_Kd") {
                std::string textFileName;
                iss >> textFileName;
                LoadTexture("PoolBalls/" + textFileName);
            }
        }
        file.close();
    }

    // Carrega textura usando stb_image
    void ObjModelLoader::LoadTexture(const std::string& texturePath) {
        
        glActiveTexture(GL_TEXTURE0);

        GLuint id = texCounter;
        textureID = id;
        
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);        
        
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                        nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
            
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
        }
        else std::cerr << "Falha ao carregar textura: " << texturePath << " - " << stbi_failure_reason() << std::endl;
    }

} // namespace PoolGame3D