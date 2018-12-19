#version 330 core

in vec2 UV;

// Output data
out vec3 color;

uniform sampler2D myTextureSample;

void main()
{
	color = texture( myTextureSample, UV/512.0 ).rgb;
}