#pragma once

#include <LimbicTypes.h>
//#include "RenderSystem.h"

#include <fbxsdk.h>
#include <vulkan/vulkan.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

enum EMemoryLocation
{
	eMemoryLocationHost,
	eMemoryLocationDevice,
	eMemoryLocationSize
};

/* Maps EMemoryLocation enums to the Vulkan memory property flags. */
const VkMemoryPropertyFlags memoryLocationVkFlags[2] = {VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

enum EMemoryBlockUsage
{
	eMemoryBlockUsageGeometry,
	eMemoryBlockUsageUniforms,
	eMemoryBlockUsageImages,
	eMemoryBlockUsageStaging,
	eMemoryBlockUsageSize
};

/* Maps EMemoryBlockUsage enums to the Vulkan buffer usage flags. */
const VkBufferUsageFlags memoryBlockUsageVkFlags[4] = {
	VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT};

/* Maps EMemoryBlockUsage enums to EMemoryLocation enums. */
const EMemoryLocation memoryBlockUsageLocation[4] = {
	eMemoryLocationDevice, eMemoryLocationHost, eMemoryLocationDevice, eMemoryLocationHost};





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

	//---- Vulkan GPU memory management. ----


	FbxManager* fbxManager;
	FbxIOSettings* fbxIOSettings;
	//RenderSystem* renderSystem;
	std::unordered_map<std::string, uint32_t> resourceLookup;
};
