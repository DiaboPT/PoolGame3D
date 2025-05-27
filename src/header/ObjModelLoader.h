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

        // Carrega o modelo OBJ. Se ignoreMtl for true, não tenta carregar o MTL.
        bool Load(const std::string& obj_model_filepath, bool ignoreMtl = false);

        // Envia os dados para a GPU (VAO, VBO, EBO, textura)
        void Install();

        void Render(const glm::vec3& position, const glm::vec3& orientation, GLuint shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

        void setMaterialAndTexture(const std::string& mtlPath, const std::string& texturePath);

    private:
        std::vector<float> vertices; 
        std::vector<unsigned int> indices;
        GLuint VAO, VBO, EBO;
        GLuint textureID;

        // Funções auxiliares
        bool LoadOBJ(const std::string& path, std::string& mtlFile);
        bool LoadMTL(const std::string& path, std::string& textureFile);
        bool LoadTexture(const std::string& texturePath);
    };

} // namespace PoolGame3D