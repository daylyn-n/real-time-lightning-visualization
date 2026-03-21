#ifndef MAP_RENDERER_HPP
#define MAP_RENDERER_HPP

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "glad/glad.h"
#include "VBO.hpp"
#include "VAO.hpp"

/*
 * purpose of this class is to be able to create and bind
 * VAOs and VBOs to draw a map, and to apply textures on that map
 * we also need to call for a mapShader program to draw the tex coords and
 * position of the map, and later manipulate the view of it
 * */
class MapRenderer
{
    public:
        MapRenderer(const ShaderProgram &shader);
        ~MapRenderer();
        void DrawMap(Texture2D &tex, glm::vec2 pos,
                    glm::vec2 size = glm::vec2(10.0f, 10.0f),
                    glm::vec3 color = glm::vec3(1.0f));

    private:
        std::unique_ptr<ShaderProgram> shader_;
        VAO mapVAO_;
        VBO mapVBO_;
        void InitData();
         
};
#endif


