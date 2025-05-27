#include "ShadersSources.h"

namespace PoolGame3D {

// Shader para a mesa (paralelepípedo colorido com iluminação e sombra nas pontas)
const char* vertexShaderTable = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aNormal;
    out vec3 faceNormal;
    out vec3 vertexPos;
    out vec3 fragNormal;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
      gl_Position = projection * view * model * vec4(aPos, 1.0);
      faceNormal = aNormal;
      vertexPos = aPos;
      fragNormal = mat3(transpose(inverse(model))) * aNormal;
    }
)";

const char* fragmentShaderTable = R"(
    #version 330 core
    in vec3 faceNormal;
    in vec3 vertexPos;
    in vec3 fragNormal;
    out vec4 FragColor;

    // Cores organizadas
    vec3 green = vec3(0.22, 0.45, 0.22); // verde (Cima)
    vec3 brown1 = vec3(0.30, 0.18, 0.08); // marrom 1 (Direita)
    vec3 brown2 = vec3(0.45, 0.28, 0.10); // marrom 2 (Esquerda)
    vec3 brown3 = vec3(0.36, 0.22, 0.09); // marrom 3 (Baixo)
    vec3 brown4 = vec3(0.40, 0.25, 0.12); // marrom 4 (Frente)
    vec3 brown5 = vec3(0.32, 0.20, 0.07); // marrom 5 (Trás)

    uniform vec3 lightPos;
    uniform vec3 lightColor;
    uniform vec3 viewPos;

    void main() {
        vec3 n = normalize(faceNormal);
        vec3 color;

        if (distance(n, vec3(0.0, 1.0, 0.0)) < 0.1) {
            color = green; // verde (Cima)
        } else if (distance(n, vec3(1.0, 0.0, 0.0)) < 0.1) {
            color = brown1; // marrom 1 (Direita)
        } else if (distance(n, vec3(-1.0, 0.0, 0.0)) < 0.1) {
            color = brown2; // marrom 2 (Esquerda)
        } else if (distance(n, vec3(0.0, -1.0, 0.0)) < 0.1) {
            color = brown3; // marrom 3 (Baixo)
        } else if (distance(n, vec3(0.0, 0.0, 1.0)) < 0.1) {
            color = brown4; // marrom 4 (Frente)
        } else if (distance(n, vec3(0.0, 0.0, -1.0)) < 0.1) {
            color = brown5; // marrom 5 (Trás)
        } else {
            color = vec3(1,0,1); // debug
        }

        // Iluminação Phong
        vec3 norm = normalize(fragNormal);
        vec3 lightDir = normalize(lightPos - vertexPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        float specularStrength = 0.3;
        vec3 viewDir = normalize(viewPos - vertexPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
        vec3 specular = specularStrength * spec * lightColor;
        color = (ambient + diffuse + specular) * color;

        // Sombra nas pontas: mistura com cinza conforme distância do centro
        float dist = length(vertexPos); // 0 no centro, ~0.87 nos cantos
        float sombra = smoothstep(0.5, 0.87, dist); // começa a escurecer a partir de 0.5
        vec3 gray = vec3(0.25); // tom de cinza para desaturar
        color = mix(color, gray, sombra * 0.6); // 0.6 controla a força da sombra

        FragColor = vec4(color, 1.0);
    }
)";

// Shader para as bolas (esfera com iluminação Phong)
const char* vertexShaderBall = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aNormal;
    layout(location = 2) in vec2 aTexCoord;
    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        vec4 worldPos = model * vec4(aPos, 1.0);
        FragPos = worldPos.xyz;
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoord = aTexCoord;
        gl_Position = projection * view * worldPos;
    }
)";

const char* fragmentShaderBall = R"(
    #version 330 core
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;
    out vec4 FragColor;
    
    uniform vec3 lightPos;
    uniform vec3 lightColor;
    uniform vec3 viewPos;
    uniform bool useTexture;
    uniform sampler2D ballTexture;
    
    // Uniforms
    uniform bool ambientEnabled;
    uniform vec3 ambientColor;
    uniform float ambientIntensity;

    uniform bool dirEnabled;
    uniform vec3 dirDirection;
    uniform vec3 dirColor;
    uniform float dirIntensity;

    uniform bool pointEnabled;
    uniform vec3 pointPos;
    uniform vec3 pointColor;
    uniform float pointIntensity;
    uniform float pointConstant, pointLinear, pointQuadratic;

    uniform bool spotEnabled;
    uniform vec3 spotPos;
    uniform vec3 spotDir;
    uniform vec3 spotColor;
    uniform float spotIntensity;
    uniform float spotCutOff, spotOuterCutOff;
    uniform float spotConstant, spotLinear, spotQuadratic;
    
    void main() {
        vec3 color;
        if (useTexture)
            color = texture(ballTexture, TexCoord).rgb;
        else
            color = vec3(1.0); // branco puro
        
        // Iluminação Phong
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        vec3 result = vec3(0.0);

        if (ambientEnabled)
            result += ambientColor * ambientIntensity * color;

        if (dirEnabled) {
            vec3 dirLightDir = normalize(-dirDirection);
            float diff = max(dot(norm, dirLightDir), 0.0);
            vec3 diffuse = diff * dirColor * dirIntensity;
            vec3 reflectDir = reflect(-dirLightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * dirColor * dirIntensity * 0.5;
            result += (diffuse + specular) * color;
        }

        if (pointEnabled) {
            vec3 pointLightDir = normalize(pointPos - FragPos);
            float diff = max(dot(norm, pointLightDir), 0.0);
            float distance = length(pointPos - FragPos);
            float attenuation = 1.0 / (pointConstant + pointLinear * distance + pointQuadratic * (distance * distance));
            vec3 diffuse = diff * pointColor * pointIntensity * attenuation;
            vec3 reflectDir = reflect(-pointLightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * pointColor * pointIntensity * attenuation * 0.5;
            result += (diffuse + specular) * color;
        }

        if (spotEnabled) {
            vec3 spotLightDir = normalize(spotPos - FragPos);
            float theta = dot(normalize(-spotDir), normalize(FragPos - spotPos));
            float epsilon = spotCutOff - spotOuterCutOff;
            float intensity = clamp((theta - spotOuterCutOff) / epsilon, 0.0, 1.0);
            float diff = max(dot(norm, spotLightDir), 0.0);
            float distance = length(spotPos - FragPos);
            float attenuation = 1.0 / (spotConstant + spotLinear * distance + spotQuadratic * (distance * distance));
            vec3 diffuse = diff * spotColor * spotIntensity * attenuation * intensity;
            vec3 reflectDir = reflect(-spotLightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * spotColor * spotIntensity * attenuation * intensity * 0.5;
            result += (diffuse + specular) * color;
        }

        FragColor = vec4(result, 1.0);
    }
)";

} // namespace PoolGame3D

