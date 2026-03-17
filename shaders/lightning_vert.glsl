#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float aAgeNorm;
layout (location = 2) in float aIntensity;
layout (location = 3) in float aPolarity;

out float vAgeNorm;
out float vPolarity;

uniform mat4 uProjection;
uniform float uBasePointSize;
uniform float uSizeScale;

void main()
{
    vAgeNorm = aAgeNorm;
    vPolarity = aPolarity;

    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
    gl_PointSize = uBasePointSize + (clamp(aIntensity, 0.0, 1.0) * uSizeScale);
}
