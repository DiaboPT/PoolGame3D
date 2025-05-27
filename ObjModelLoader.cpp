#include "Header.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ObjModelLoader::ObjModelLoader() {}
ObjModelLoader::~ObjModelLoader() {
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_uvs);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &textureID);
}

bool ObjModelLoader::Load(const std::string objPath) {
    std::ifstream file(objPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << objPath << "\n";
        return false;
    }

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::string mtlFile;
    std::string line;
    std::string directory = objPath.substr(0, objPath.find_last_of("/\\") + 1);

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "mtllib") {
            iss >> mtlFile;
        }
        else if (prefix == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y; // Flip V
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f") {
            unsigned int vi[3], uvi[3], ni[3];
            char slash;
            for (int i = 0; i < 3; ++i)
                iss >> vi[i] >> slash >> uvi[i] >> slash >> ni[i];
            for (int i = 0; i < 3; ++i) {
                vertices.push_back(temp_vertices[vi[i] - 1]);
                uvs.push_back(temp_uvs[uvi[i] - 1]);
                normals.push_back(temp_normals[ni[i] - 1]);
            }
        }
    }

    file.close();

    // Load texture from MTL
    std::ifstream mtl(directory + mtlFile);
    if (!mtl.is_open()) {
        std::cerr << "MTL file not found: " << mtlFile << "\n";
        return false;
    }

    std::string textureFile;
    while (getline(mtl, line)) {
        if (line.find("map_Kd") != std::string::npos) {
            std::istringstream iss(line);
            std::string ignore;
            iss >> ignore >> textureFile;
            break;
        }
    }

    int width, height, channels;
    std::string fullTexturePath = directory + textureFile;
    unsigned char* data = stbi_load(fullTexturePath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << fullTexturePath << "\n";
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return true;
}

void ObjModelLoader::Install() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &vbo_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); // normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void ObjModelLoader::Render(glm::vec3 position, glm::vec3 orientation) {
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Model matrix (basic example, your shader needs MVP matrix support)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(orientation.y), glm::vec3(0, 1, 0));

    // You’d typically send model matrix to your shader here

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}
