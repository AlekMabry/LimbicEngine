#pragma once

#include "LimbicTypes.h"
//#include "RenderSystem.h"

#include <fbxsdk.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class ResourceSystem
{
public:
	ResourceSystem();

	~ResourceSystem();

	RStaticMesh RequestStaticMesh(std::string& filename, std::string& nodeName);

	RTexture RequestTexture(std::string& filename);

	RMaterial RequestMaterial(std::string& baseColorFilename, std::string& normalFilename, std::string& propertiesFilename);

protected:
	/** Recursive search by node name. */
	FbxNode* GetFbxNode(FbxNode* root, std::string& nodeName) const;

	void PrintNode(FbxNode* node, int indent = 0);

	SMesh* LoadMesh(std::string& filename, std::string& nodeName);

	void LoadTextureKTX2(std::string& filename, void* buffer);

	FbxManager* fbxManager;
	FbxIOSettings* fbxIOSettings;
	//RenderSystem* renderSystem;
	std::unordered_map<std::string, uint32_t> resourceLookup;
};
