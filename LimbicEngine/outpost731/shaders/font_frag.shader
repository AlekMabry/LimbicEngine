#version 330 core
in vec2 UV;

uniform mat4 MATRIX_WORLD_VIEW_PROJECTION;
uniform vec2 in_texDimensions;
uniform vec3 in_color;
uniform sampler2D fontAtlas;

out vec3 color;

void main()
{    
    vec4 sampled = fontAtlas(UV / in_texDimensions).rgb;
    color = in_color * sampled;
}