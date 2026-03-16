#version 410 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = uProjection * uModel * vec4(vertex.xy, 0.0, 1.0);
}
