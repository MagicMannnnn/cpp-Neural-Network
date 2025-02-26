#version 330 core

in vec2 TexCoords;
flat in int layer;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoords);
}