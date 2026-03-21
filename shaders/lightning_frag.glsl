#version 410 core
in float vAgeNorm;
in float vPolarity;
out vec4 color;

vec3 polarityColor(float polarity)
{
    if (polarity >= 0.0)
    {
        return vec3(1.0, 0.0, 0.0);
    }

    return vec3(0.0, 1.0, 0.0);
}

void main()
{
    vec2 centered = (gl_PointCoord * 2.0) - 1.0;
    float radiusSq = dot(centered, centered);
    if (radiusSq > 1.0)
    {
        discard;
    }

    vec3 strikeColor = polarityColor(vPolarity);
    float edgeFalloff = smoothstep(1.0, 0.2, radiusSq);
    float ageFade = max(0.25, 1.0 - clamp(vAgeNorm, 0.0, 1.0));
    float alpha = edgeFalloff * ageFade;

    color = vec4(strikeColor, 1.0f);
}
