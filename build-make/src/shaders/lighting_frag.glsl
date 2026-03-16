#version 410 core

out vec4 color;

in vec3 Normal;
in vec3 Position;

uniform vec3 u_lightPos;
uniform vec3 u_objColor;
uniform vec3 u_lightColor;

void main()
{
    float ambientStr = 0.5f;
    vec3 ambient = ambientStr * u_lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - Position);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse) * u_objColor;
    color = vec4(result, 1.0f);
}
