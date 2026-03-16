#ifndef VBO_HPP
#define VBO_HPP

#include "../include/glad/glad.h"
#include <vector>

class VBO
{
public:
    GLuint VBOID {};

    // Create the VBO on the GPU
    void create()
    {
        glGenBuffers(1, &VBOID);
    }

    void bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBOID);
    }

    static void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Upload vertex data to the GPU
    void setData(const std::vector<GLfloat>& data, GLenum usage = GL_STATIC_DRAW)
    {
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), usage);
    }

    void destroy()
    {
        if (VBOID != 0) {
            glDeleteBuffers(1, &VBOID);
            VBOID = 0;
        }
    }
};

#endif
