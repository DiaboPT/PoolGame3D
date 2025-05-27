#include "Light.h"

using namespace PoolGame3D;

Light::Light() : position(0.0f, 2.0f, 2.0f), color(1.0f, 1.0f, 1.0f), intensity(1.0f) {}

Light::Light(const glm::vec3& pos, const glm::vec3& color, float intensity)
    : position(pos), color(color), intensity(intensity) {} 