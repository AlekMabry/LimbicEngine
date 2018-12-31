#version 330 core

in vec2 UV;
in vec2 texDimensions;

// Output data
out vec3 color;

uniform sampler2D myTextureSample;

void main()
{
	vec2 tex_uv = vec2(UV.x/texDimensions.x, UV.y/texDimensions.y);
	color = texture( myTextureSample, tex_uv ).rgb;
}