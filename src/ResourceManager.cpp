#include "../include/ResourceManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, ShaderProgram>   ResourceManager::Shaders;

ShaderProgram& ResourceManager::LoadShader(const std::string &vertPath,
                                        const std::string &fragPath,
                                        std::string name)
{
    Shaders.insert_or_assign(name, loadShaderFromFile(vertPath, fragPath));
    return Shaders.at(name);
}

ShaderProgram& ResourceManager::GetShader(const std::string &name)
{
    return Shaders.at(name);
}
Texture2D& ResourceManager::LoadTexture(const std::string &file,
                                        bool alpha,
                                        std::string name)
{
    Textures.insert_or_assign(name, loadTextureFromFile(file, alpha));
    return Textures.at(name);
}

Texture2D& ResourceManager::GetTexture(const std::string &name)
{
    return Textures.at(name);
}

void ResourceManager::Clear()
{
    for(auto it : Shaders)
        glDeleteProgram(it.second.GraphicsPipelineShaderProgram);
    for(auto it : Textures)
        glDeleteTextures(1, &it.second.ID);
}

ShaderProgram ResourceManager::loadShaderFromFile(const std::string &vertPath,
                                                const std::string &fragPath)
{

    ShaderProgram shader(vertPath, fragPath);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const std::string &file, bool alpha)
{
    Texture2D texture;
    int width, height, nrChannels;
    const int requestedChannels = alpha ? STBI_rgb_alpha : 0;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, requestedChannels);
    if (data == nullptr)
    {
        std::cout << "Failed to load texture file: " << file;
        const char* reason = stbi_failure_reason();
        if (reason != nullptr)
        {
            std::cout << " (" << reason << ")";
        }
        std::cout << std::endl;

        // Fallback checker texture so rendering still works even if file decoding fails.
        static const unsigned char fallbackRgba[] = {
            255, 0, 255, 255,    0, 0, 0, 255,
            0, 0, 0, 255,        255, 0, 255, 255,
        };

        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
        texture.Generate(2, 2, fallbackRgba);
        return texture;
    }

    const int loadedChannels = requestedChannels != 0 ? requestedChannels : nrChannels;
    switch (loadedChannels)
    {
        case 4:
            texture.internalFormat = GL_RGBA;
            texture.imageFormat = GL_RGBA;
            break;
        case 3:
            texture.internalFormat = GL_RGB;
            texture.imageFormat = GL_RGB;
            break;
        case 2:
            texture.internalFormat = GL_RG;
            texture.imageFormat = GL_RG;
            break;
        case 1:
            texture.internalFormat = GL_RED;
            texture.imageFormat = GL_RED;
            break;
        default:
            texture.internalFormat = GL_RGBA;
            texture.imageFormat = GL_RGBA;
            break;
    }

    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}
