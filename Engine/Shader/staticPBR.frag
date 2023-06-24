#version 450

layout(location = 0) in vec2 inTextureUV;

layout(location = 0) out vec4 diffuseColor;

layout(binding = 0) uniform sampler2D baseColor;
layout(binding = 1) uniform sampler2D normal;
layout(binding = 2) uniform sampler2D properties;

void main()
{
    vec2 uv = vec2(inTextureUV.x, 1.0f - inTextureUV.y);
    diffuseColor = texture(baseColor, uv);
}