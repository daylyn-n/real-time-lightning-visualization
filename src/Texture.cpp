#include <iostream>
#include "../include/Texture.hpp"

Texture2D::Texture2D()
    : ID(0), width(0), height(0), internalFormat(GL_RGB),
    imageFormat(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT),
    filterMin(GL_LINEAR), filterMax(GL_LINEAR)
{
}

void Texture2D::Generate(GLuint w, GLuint h, const unsigned char* data)
{
    width = w;
    height =h;

    if (this->ID == 0)
    {
        glGenTextures(1, &this->ID);
    }

    // create textures
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, w, h, 0,
            this->imageFormat, GL_UNSIGNED_BYTE, data);
    // set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    // unbind tex
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}
