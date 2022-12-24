#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using namespace glm;

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned long long uint64;
typedef long int64;

typedef unsigned int RMaterial;
typedef unsigned int RTexture;
typedef unsigned int RStaticMesh;

struct STexture
{
};

struct SMaterial
{
	STexture* albedo;
	STexture* normal;
	STexture* material;
};

struct SStaticVertex
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec2 textureUV;
	vec2 lightmapUV;
};

struct SMesh
{
	uint32 vertexCount;
	std::vector<SStaticVertex> vertices;
	std::vector<uint32> indices;
};