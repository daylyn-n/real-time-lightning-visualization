#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad/glad.h"
#include <string>

/*
 *class to store and configure 2d textures for our map
 * */

class Texture2D
{
    public:
        GLuint ID;
        GLuint width, height;
        GLuint internalFormat;
        GLuint imageFormat;
        // config
        GLuint wrapS;
        GLuint wrapT;
        GLuint filterMin;
        GLuint filterMax;

        Texture2D();
        
        void Generate(GLuint width, GLuint height, const unsigned char* data);
        void Bind() const;
};
#endif 
