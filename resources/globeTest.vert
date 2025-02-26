#version 330 core

layout (location = 0) in vec3 aPos;      // Vertex position in local space
layout (location = 1) in vec3 aNormal;   // Vertex normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates
layout (location = 3) in vec3 aTangent;  // Tangent vector
layout (location = 4) in vec3 aBitangent; // Bitangent vector

uniform mat4 model;        // Model matrix
uniform mat4 view;         // View matrix
uniform mat4 projection;   // Projection matrix

// Hex grid uniforms
uniform float hexSize; // Size of a single hex cell
uniform vec2 gridSize; // Number of hexagons in width and height

out vec3 FragPos;          // Fragment position in world space
out vec3 Normal;           // Normal vector in world space
out vec2 TexCoords;        // Texture coordinates

vec3 hexToSphere(vec2 hexCoords) {
    // Convert hex grid coordinates to latitude and longitude
    float u = hexCoords.x / gridSize.x * 2.0 - 1.0; // Normalize to [-1,1]
    float v = hexCoords.y / gridSize.y * 2.0 - 1.0; // Normalize to [-1,1]
    
    float longitude = u * 3.14159265359; // Convert to radians (-? to ?)
    float latitude = v * 1.57079632679;  // Convert to radians (-?/2 to ?/2)
    
    // Convert spherical coordinates to Cartesian
    float x = cos(latitude) * cos(longitude);
    float y = sin(latitude);
    float z = cos(latitude) * sin(longitude);
    
    return vec3(x, y, z);
}

void main() {
    // Convert texture coordinates to hexagonal grid space
    vec2 hexCoords = aTexCoords * gridSize;
    
    // Project hex grid onto sphere
    vec3 sphericalPos = hexToSphere(hexCoords);
    
    // Apply model transformation
    FragPos = vec3(model * vec4(sphericalPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    TexCoords = aTexCoords;
    
    // Transform to clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
