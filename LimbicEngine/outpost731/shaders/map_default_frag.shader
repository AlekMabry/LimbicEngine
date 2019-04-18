#version 330 core

in vec2 uv;
in vec3 norm;
in vec2 texDimensions;

// Output data
out vec3 color;

uniform sampler2D myTextureSample;

void main()
{
	vec3 norm_shifted = (norm/2)+0.5;
	vec2 tex_uv = vec2(uv.x/texDimensions.x, uv.y/texDimensions.y);
	color = vec3(0, 0, norm_shifted.b);  // texture( myTextureSample, tex_uv ).rgb;
}