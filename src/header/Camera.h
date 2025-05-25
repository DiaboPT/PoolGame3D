#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PoolGame3D {
    class Camera {
    public:
    Camera();
    void update();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;
    
    // Getters e setters
    float getFOV() const { return fov; }
    void setFOV(float newFOV) { fov = newFOV; }
    float getDistance() const { return distance; }
    void setDistance(float newDistance) { distance = newDistance; }
    
    // Controles da câmera
    void rotate(float yaw, float pitch);
    void zoom(float amount);

    private:
        float distance = 5.0f;
        float pitch = -30.0f;
        float yaw = 0.0f;
        float fov = 45.0f;
        
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up;
    };
} 