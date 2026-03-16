#version 410 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;
uniform mat4 u_ViewMatrix;

out vec3 Normal;
out vec3 Position;
void main()
{
    vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);

    gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
    Position = vec3(u_ModelMatrix * vec4(position, 1.0f));
    // Transform normals properly using the normal matrix (handles non-uniform scaling)
    Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
}
