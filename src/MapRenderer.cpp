#include "../include/MapRenderer.hpp"
#include <cstdlib>
#include <memory>
#include <vector>

MapRenderer::MapRenderer(const ShaderProgram &shader)
{
    shader_ = std::make_unique<ShaderProgram>(shader);
    InitData();
}

MapRenderer::~MapRenderer()
{
    mapVBO_.destroy();
    mapVAO_.destroy();
}

void MapRenderer::InitData()
{
    std::vector<GLfloat> vertices
    {
        // pos      // tex
        0.0f, 0.0f,     0.0f, 0.0f,
        1.0f, 0.0f,     1.0f, 0.0f,
        1.0f, 1.0f,     1.0f, 1.0f,

        0.0f, 0.0f,     0.0f, 0.0f,
        1.0f, 1.0f,     1.0f, 1.0f,
        0.0f, 1.0f,     0.0f, 1.0f,
    };

    mapVAO_.create();
    mapVAO_.bind();

    mapVBO_.create();
    mapVBO_.bind();
    mapVBO_.setData(vertices);

    mapVAO_.setVertexAttrib(0, 4, 4 * sizeof(GLfloat), 0);

    VAO::unbind();
    VBO::unbind();

}
void MapRenderer::DrawMap(Texture2D &tex, glm::vec2 pos,
                        glm::vec2 size, glm::vec3 color)
{
    (void)color;
    shader_->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader_->setMat4("uModel", model);
    glActiveTexture(GL_TEXTURE0);
    tex.Bind();
    mapVAO_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    VAO::unbind();
}
