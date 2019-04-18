#version 330 core
layout (location = 0) in vec2 in_vert;
layout (location = 1) in vec2 in_uv;

out vec2 UV;

uniform mat4 MATRIX_WORLD_VIEW_PROJECTION;
uniform vec2 in_texDimensions;
uniform vec3 in_color;

void main()
{
    UV = vertexUV;
    gl_Position = MATRIX_WORLD_VIEW_PROJECTION * vec4(in_vert, 0.0, 1.0);
}  