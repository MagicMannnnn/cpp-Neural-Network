#version 330 core

layout (location = 0) in vec3 aPos;      // Vertex position
layout (location = 1) in vec3 aNormal;   // Vertex normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates
layout (location = 3) in vec3 aTangent;  // Tangent vector
layout (location = 4) in vec3 aBitangent; // Bitangent vector

uniform mat4 model;        // Model matrix
uniform mat4 view;         // View matrix
uniform mat4 projection;   // Projection matrix

out vec3 FragPos;          // Fragment position in world space
out vec3 Normal;           // Normal vector in world space
out vec2 TexCoords;        // Texture coordinates

void main() {
    // Apply model, view, and projection transformations
    FragPos = vec3(model * vec4(aPos, 1.0)); // Calculate world-space position
    Normal = mat3(transpose(inverse(model))) * aNormal; // Correctly transform normals
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0); // Transform into clip space
}
