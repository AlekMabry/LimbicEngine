#version 450

layout(push_constant) uniform SConstants
{
    mat4 camera;
    mat4 model;
} constants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec2 inTextureUV;
layout(location = 5) in vec2 inLightmapUV;

layout(location = 0) out vec2 outTextureUV;

void main() {
    gl_Position = constants.camera * constants.model * vec4(inPosition, 1.0);
    outTextureUV = inTextureUV;
}