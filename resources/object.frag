#version 330 core

in vec3 FragPos;      // Fragment position in world space
in vec3 Normal;       // Normal vector in world space
in vec2 TexCoords;    // Texture coordinates

uniform sampler2D texture_diffuse1; // Diffuse texture
uniform sampler2D texture_specular1; // Specular texture
uniform vec3 lightPos;              // Light position
uniform vec3 viewPos;               // Camera/view position
uniform vec3 lightColor;            // Light color

out vec4 FragColor;

void main() {
    // Ambient lighting
    vec3 ambient = 0.1 * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texture(texture_diffuse1, TexCoords).rgb;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * texture(texture_specular1, TexCoords).rgb;

    // Combine results
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
