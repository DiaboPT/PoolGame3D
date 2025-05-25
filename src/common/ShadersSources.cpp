#include "ShadersSources.h"

namespace PoolGame3D {

// Shader para a mesa (paralelepípedo colorido)
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;
    out vec3 ourColor;
    uniform mat4 MVP;
    void main() {
      gl_Position = MVP * vec4(aPos, 1.0);
      ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main() {
      FragColor = vec4(ourColor, 1.0);
    }
)";

// Shader para a bola (esfera com textura)
const char* vertexShaderTexture = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoord = aTexCoord;
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderTexture = R"(
    #version 330 core
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;

    out vec4 FragColor;

    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform vec3 lightColor;
    uniform sampler2D texture1;

    void main() {
        // Propriedades do material
        vec3 materialColor = texture(texture1, TexCoord).rgb;
        
        // Luz ambiente
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        
        // Luz difusa
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // Luz especular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        // Resultado final
        vec3 result = (ambient + diffuse + specular) * materialColor;
        FragColor = vec4(result, 1.0);
    }
)";

} // namespace PoolGame3D 