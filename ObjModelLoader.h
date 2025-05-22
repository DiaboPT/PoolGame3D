#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace PoolGame3D {

    class ObjModelLoader {
    public:
        ObjModelLoader();
        ~ObjModelLoader();

        // Carrega o modelo .obj e o .mtl associado
        bool Load(const std::string& obj_model_filepath);

        // Envia os dados para a GPU (VAO, VBO, EBO, textura)
        void Install();

        // Renderiza o modelo na posição e orientação desejada
        void Render(const glm::vec3& position, const glm::vec3& orientation, GLuint shaderProgram, const glm::mat4& viewProj);

    private:
        // Dados do modelo
        std::vector<float> vertices; // pos(3), normal(3), texcoord(2)
        std::vector<unsigned int> indices;
        GLuint VAO, VBO, EBO;
        GLuint textureID;

        // Funções auxiliares
        bool LoadOBJ(const std::string& path, std::string& mtlFile);
        bool LoadMTL(const std::string& path, std::string& textureFile);
        bool LoadTexture(const std::string& texturePath);
    };

} // namespace PoolGame3D