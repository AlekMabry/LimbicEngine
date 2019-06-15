#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 uv;
in vec3 norm;
in vec3 pos;

in vec2 texDimensions;

uniform sampler2D myTextureSample;

void main()
{
	vec2 tex_uv = vec2(uv.x/texDimensions.x, uv.y/texDimensions.y);
	gPosition = pos;
	gNormal = normalize(norm);
	gAlbedoSpec.rgb = texture(myTextureSample, tex_uv).rgb;
	gAlbedoSpec.a = 0.5;
}