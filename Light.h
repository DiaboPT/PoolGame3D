#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "LoadShaders.h"
#include "ObjModelLoader.h"

namespace lighting
{
	bool light[] = { true, true, true, true };

	void Lights(PoolGame3D::ObjModelLoader* obj, GLuint);
	void OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	glm::vec3 black = glm::vec3(0, 0, 0);

	void lighting::Lights(PoolGame3D::ObjModelLoader* obj, GLuint shader)
	{
		GLuint shaderProgram = shader;
		
		//ambient
		if (light[0])
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "ambientLight.ambient"), 1,
				glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		else glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "ambientLight.ambient"), 1,
			glm::value_ptr(lighting::black));
		
		if (light[1])
		{
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, -1.0)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(10.0, 10.0, 10.0)));
		}
		else
		{
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.direction"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.ambient"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.diffuse"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "directionalLight.specular"), 1, glm::value_ptr(lighting::black));
		}

		//pointLight
		if (light[2])
		{
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.position"), 1, glm::value_ptr(glm::vec3(.0f, 1.0f, 1.0f)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.ambient"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.constant"), 1.0f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.linear"), 0.06f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.quadratic"), 0.02f);
		}
		else
		{
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.ambient"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.diffuse"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.specular"), 1, glm::value_ptr(lighting::black));
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.constant"), 1.0f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.linear"), 0.06f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "pointLight.quadratic"), 0.02f);
		}

		//SpotLight
		if (light[3])
		{
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.position"), 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(2.0, 2.0, 2.0)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(10.0, 10.0, 10.0)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(10.0, 10.0, 10.0)));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.direction"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.angle"), 0.00005f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.constant"), 1.0f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.linear"), 0.06f);
			glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.quadratic"), 0.02f);
		}
		else
		{
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.ambient"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.diffuse"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.specular"), 1, glm::value_ptr(lighting::black));
			glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "spotLight.direction"), 1, glm::value_ptr(lighting::black));
		}

		//Material Props from the obj
		glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(obj->ka));
		glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(obj->kd));
		glProgramUniform3fv(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(obj->ks));
		glProgramUniform1f(shaderProgram, glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "material.shininess"), obj->ns);
	}

	void lighting::OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//ambiente 1
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			std::cout << "test" << std::endl;
			if (light[0]) light[0] = false;
			else light[0] = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			if (light[1]) light[1] = false;
			else light[1] = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			if (light[2]) light[2] = false;
			else light[2] = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			if (light[3]) light[3] = false;
			else light[3] = true;
		}
	}
}