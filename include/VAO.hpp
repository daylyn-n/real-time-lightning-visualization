#ifndef VAO_HPP
#define VAO_HPP

#include "../include/glad/glad.h"
#include <initializer_list>
#include <iostream>
class VAO
{
public:
    struct VertexAttributeSpec
    {
        GLuint index {};
        GLint size {};
        GLenum type {GL_FLOAT};
        GLboolean normalized {GL_FALSE};
        GLsizei stride {};
        size_t offset {};
    };

    GLuint VAOID {};

    // Create the VAO on the GPU
    void create()
    {
        glGenVertexArrays(1, &VAOID);
    }

    void bind()
    {
        glBindVertexArray(VAOID);
    }

    static void unbind()
    {
        glBindVertexArray(0);
    }

    // Configure a vertex attribute pointer
    // index: attribute location in shader
    // size: number of components (e.g., 3 for vec3)
    // stride: bytes between consecutive vertices
    // offset: byte offset of this attribute in the vertex
    void setVertexAttrib(GLuint index, GLint size, GLsizei stride, size_t offset)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
    }

    void setVertexAttrib(const VertexAttributeSpec& spec)
    {
        glEnableVertexAttribArray(spec.index);
        glVertexAttribPointer(spec.index, spec.size, spec.type, spec.normalized, spec.stride, (GLvoid*)spec.offset);
    }

    void setVertexLayout(std::initializer_list<VertexAttributeSpec> attributes)
    {
        for (const VertexAttributeSpec& attribute : attributes) {
            setVertexAttrib(attribute);
        }
    }

    void destroy()
    {
        if (VAOID != 0) {
            glDeleteVertexArrays(1, &VAOID);
            VAOID = 0;
        }
    }
};

#endif
