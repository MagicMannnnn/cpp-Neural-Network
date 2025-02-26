#ifndef TEXTURES_HPP
#define TEXTURES_HPP
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>


enum TEXTURES {
    STONE = 0,
    NONE
};

/*
struct Texture {
    unsigned int id;       // OpenGL texture ID
    std::string type;      // Diffuse, specular, etc.
    std::string path;      // File path (for caching purposes)
};
*/

struct Texture {
    unsigned int id;       // OpenGL texture ID (0 for color)
    std::string type;      // Type of texture (diffuse, specular, etc. or color name)
    std::string path;      // File path (for caching purposes)

    // Additional fields for storing color data
    float r, g, b, a;      // Color values (used if id is 0)

    // Constructor for a regular texture (loaded from file)
    Texture(unsigned int textureID, const std::string& textureType, const std::string& texturePath)
        : id(textureID), type(textureType), path(texturePath), r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

    // Constructor for color textures (when there's no actual image)
    Texture(const std::string& colorType, float red, float green, float blue, float alpha)
        : id(0), type(colorType), path(""), r(red), g(green), b(blue), a(alpha) {}
};

namespace Textures {

    unsigned int stoneTexture;

    void loadTextures() {

        // Generate and bind a single texture
        glGenTextures(1, &stoneTexture);
        glBindTexture(GL_TEXTURE_2D, stoneTexture);

        // Texture path
        std::string texturePath = RESOURCES_PATH "textures/stone.jpg";

        int width, height, nrChannels;

        // Load the texture data
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            // Upload texture data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Set texture wrapping and filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);

            // Free the image memory
            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
        }

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);


    }
}

   
#endif