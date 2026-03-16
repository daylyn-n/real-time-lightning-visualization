#version 410 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D uImage;

void main()
{
    color = vec4(1.0) * texture(uImage, TexCoords);
}
