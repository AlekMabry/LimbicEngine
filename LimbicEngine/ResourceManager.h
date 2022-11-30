#pragma once

#include "LimbicTypes.h"

#include <fbxsdk.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

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
	float position[3];
	float normal[3];
	float tangent[3];
	float bitangent[3];
	float uv[2];
};

struct SMesh
{
	uint32 vertexCount;
	std::vector<SStaticVertex> vertices;
	std::vector<uint32> indices;
};

class ResourceManager
{
public:
	ResourceManager();
	void PrintNode(FbxNode* node, int indent = 0);
	SMesh* LoadMesh(std::string &filename, std::string &nodeName);
	~ResourceManager();

protected:
	/** Recursive search by node name. */
	FbxNode* GetFbxNode(FbxNode *root, std::string &nodeName) const;

	FbxManager* fbxManager;
	FbxIOSettings* fbxIOSettings;
	std::unordered_map<std::string, std::unique_ptr<SMesh>> meshes;
};
