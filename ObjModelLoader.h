#pragma once
#include "Header.h"

class ObjModelLoader {
public:
    ObjModelLoader();
    ~ObjModelLoader();

    bool Load(const std::string obj_model_filepath);
    void Install();
    void Render(glm::vec3 position, glm::vec3 orientation);
    void NormalizeToUnit();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    GLuint vao = 0, vbo_vertices = 0, vbo_uvs = 0, vbo_normals = 0;
    GLuint textureID = 0;
};
