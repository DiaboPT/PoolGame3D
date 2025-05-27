#version 330 core
out vec4 FragColor;

flat in vec3 fragNormal;

void main() {
    vec3 lightDir = normalize(vec3(0,1,0));
    float diff = max(dot(normalize(fragNormal), lightDir), 0.0);
    vec3 color = vec3(0.00, 0.67, 0.00);
    FragColor = vec4(color * diff, 1.0);
}
