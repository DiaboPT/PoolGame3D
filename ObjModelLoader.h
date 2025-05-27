#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define GLFW_USE_DWM_SWAP_INTERVAL

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

namespace PoolGame3D {

    class ObjModelLoader {
    public:
        ObjModelLoader();
        ~ObjModelLoader();

        // Carrega o modelo .obj e o .mtl associado
        void Load(const std::string obj_model_filepath, GLuint sPos, GLuint sNormal, GLuint sTextCoor, GLuint textureBind, GLuint shader, int counter);

        // Envia os dados para a GPU (VAO, VBO, EBO, textura)
        void Install(void);

        // Renderiza o modelo na posi��o e orienta��o desejada
        void Render(glm::vec3 position, glm::vec3 orientation, glm::mat4 modelMatrix);
        
        glm::vec3 ka, kd, ks;
        GLfloat ns;
        GLuint shaderProgram;

    private:
        // Dados do modelo
        GLuint VAO, VBO[3];
        GLuint sPos, sNormal, sTextCoord, textureBind;
        GLuint textureID;
        int texCounter;

        std::vector<glm::vec3> vertexIndices; // Position
        std::vector<glm::vec3> normalIndices; // Normals
        std::vector<glm::vec2> texcoordIndices; // Texture Coords

        // Funções auxiliares
        void LoadOBJ(const std::string& path);
        void LoadMTL(const std::string& materialFile);
        void LoadTexture(const std::string& texturePath);
    };

} // namespace PoolGame3D