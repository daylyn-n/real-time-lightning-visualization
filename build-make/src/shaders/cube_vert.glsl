#version 410 core
layout(location=0) in vec3 position;


uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;
uniform mat4 u_ViewMatrix;

void main()
{
    vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
    gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
}

