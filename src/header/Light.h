#pragma once
#include <glm/glm.hpp>

namespace PoolGame3D {

class Light {
public:
    Light();
    Light(const glm::vec3& pos, const glm::vec3& color, float intensity = 1.0f);

    void setPosition(const glm::vec3& pos) { position = pos; }
    void setColor(const glm::vec3& col) { color = col; }
    void setIntensity(float i) { intensity = i; }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct AmbientLight {
    glm::vec3 color;
    float intensity;
    bool enabled = true;
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    bool enabled = true;
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float constant, linear, quadratic;
    bool enabled = true;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float cutOff, outerCutOff;
    float constant, linear, quadratic;
    bool enabled = true;
};

} // namespace PoolGame3D 