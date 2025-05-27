#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform mat4 u_Model;

flat out vec3 fragNormal;

void main() {
    gl_Position = u_MVP * vec4(position, 1.0);
    fragNormal = mat3(transpose(inverse(u_Model))) * normal;
}
