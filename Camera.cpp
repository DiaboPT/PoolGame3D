#include "Camera.h"

using namespace cam;

Camera::Camera(float fov, float windowWidth, float windowHeight, glm::vec3 pos, glm::vec3 target)
{
	using namespace glm;
	position = pos;
	this->target = target;
	vec3 camFront = position - this->target;
	vec3 camRight = cross(camFront, vec3(0.0f, 1.0f, 0.0f));
	vec3 up = -cross(camFront, camRight);
	projection = perspective(radians(fov), windowWidth / windowHeight, 0.1f, 100.0f);
	view = lookAt(position, this->target, up);
}

Camera* Camera::instance = nullptr;

Camera* Camera::GetInstance()
{
	if (!instance) return new Camera();
	else return instance;
}

