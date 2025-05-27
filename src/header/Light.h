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

} // namespace PoolGame3D 