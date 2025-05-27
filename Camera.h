#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>

namespace cam
{
	class Camera
	{
	public:
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 position;
		glm::vec3 target;
		static Camera* GetInstance();
		Camera(float fov = 90.0f, float windowWidth = 1600.0f, float windowHeight = 900.0f, glm::vec3 pos = glm::vec3(.0f, 30.0f, 30.0f), glm::vec3 target = glm::vec3(.0f, .0f, .0f));
	private:
		static Camera* instance;
	};
}