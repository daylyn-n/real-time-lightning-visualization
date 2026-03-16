
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include "glad/glad.h"

#include "Texture.hpp"
#include "ShaderProgram.hpp"

/*
 *  ResourceManager class to define methods to load Textures and Shaders
 *
 * */

class ResourceManager
{
    public:
        // resource storage
        static std::map<std::string, ShaderProgram> Shaders;
        static std::map<std::string, Texture2D>     Textures;

        static ShaderProgram& LoadShader(const std::string &vShaderSrc,
                                        const std::string &fShaderSrc,
                                        std::string name);
        static ShaderProgram& GetShader(const std::string &name);

        static Texture2D& LoadTexture(const std::string &file, bool alpha, std::string name);
        static Texture2D& GetTexture(const std::string &name);

        static void Clear();

    private:
        ResourceManager() {}

        static ShaderProgram loadShaderFromFile(const std::string &vertPath,
                                                const std::string &fragPath);
        static Texture2D loadTextureFromFile(const std::string &file, bool alpha);

};
#endif
