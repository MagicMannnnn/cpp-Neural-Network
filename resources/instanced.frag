#version 330 core

in vec3 FragPos;      
in vec3 Normal;       
in vec2 TexCoords;    

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec4 ambientColor;

out vec4 FragColor;

void main() {
    vec3 ambient = 0.1 * lightColor * ambientColor.rgb;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseColor.rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * 0.2f * lightColor * specularColor.rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
