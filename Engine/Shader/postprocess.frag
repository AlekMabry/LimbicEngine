#version 450

#define PI 3.1415926535897932384626433832795
#define SAMPLES 32
#define WIDTH 0.8

layout(location = 0) in vec2 inTextureUV;

layout(location = 0) out vec4 outColor;

layout(input_attachment_index = 0, set = 0, binding = 3) uniform subpassInput baseColor;

layout(binding = 1) uniform sampler2D id;

void main() {

/*
    float outlineAlpha = 0.0;
	float angle = 0.0;
	for (int i = 0; i < SAMPLES; i++){
		angle += 1.0 / (float(SAMPLES) / 2.0) * PI;
		vec2 testPoint = vec2((WIDTH / u_textureRes.x)*cos(angle), (WIDTH/u_textureRes.y)*sin(angle) );
		testPoint = clamp( catUV + testPoint, u_textureBoundsUV.xy, u_textureBoundsUV.zw );
		float sampledAlpha = texture( iChannel0,  testPoint ).a;
		outlineAlpha = max( outlineAlpha, sampledAlpha );
	}
	fragColor = mix( vec4(0.0), COLOR, outlineAlpha );
*/
    vec2 uv = vec2(inTextureUV.x, inTextureUV.y);
    outColor = subpassLoad(baseColor) + (vec4(0.2, 0.0, 0.0, 0.0) * texture(id, uv));
}