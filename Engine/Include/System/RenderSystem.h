#pragma once

#include "ResourceSystem.h"

#include <LimbicTypes.h>
#include <System/IOSystem.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

struct SUniformBufferObject
{
	mat4 transform;
};

struct SPushConstants
{
	mat4 camera;
	mat4 model;
};

struct SDrawStaticPBR
{
	RStaticMesh mesh;
	RMaterial material;
	mat4 modelTransform;
};

struct SLights
{
	vec4 lights[8];
};







struct SMeshMemoryHandle
{
	EMemoryLocation location;
	uint32 vertexBlock;
	uint32 indexBlock;
	VkDeviceSize vertexOffset;
	uint32 vertices;
	VkDeviceSize indexOffset;
	uint32 indices;
};

struct SMaterialHandle
{
	VkDescriptorSet descriptor;
};

struct STextureMemoryHandle
{
	EMemoryLocation location;
	uint32 block;
	VkDeviceSize offset;
	VkDeviceSize size;
	VkImage image;
	VkImageView imageView;
	uint32 width;
	uint32 height;
	ETextureFormat format;
};

/* Handle and state of a memory block in host or device memory. */
struct SMemoryBlock
{
	EMemoryBlockUsage usage; /* Type of data the memory block stores. */
	VkBuffer buffer;		 /* Buffer that uses entire allocated memory (only defined for
								eMemoryBlockUsageGeometry/eMemoryBlockUsageUniforms).*/
	VkDeviceMemory memory;	 /* Allocated memory for the block. */
	VkDeviceSize cursor;	 /* Current offset of free memory in the block. */
	VkDeviceSize availableMemory;
	void* mappedLocation; /* Host-mapped location (only defined for eMemoryBlockUsageStaging/eMemoryBlockUsageUniforms).*/
};

const VkDeviceSize MEMORY_BLOCK_SIZE = 8 * 1024 * 1024;

class RenderSystem
{
public:
	~RenderSystem();

	void Init(const char* applicationName, VkInstance instance, IOSystem* pIO);

	/* Call to tell renderer that the window surface has been resized. */
	void Resize();

	/* Creates static mesh, provides staging buffer to write vertices and indices to. */
	void CreateStaticMesh(uint32 vertices, uint32 indices, uint32& meshHandle, SStaticVertex*& vertexBuffer, uint32*& indexBuffer);

	/* Creates texture, provides staging buffer to write image to. */
	void CreateTexture(uint32 width, uint32 height, ETextureFormat format, uint32& textureHandle, void*& textureBuffer);

	/* Creates material from texture handles. */
	void CreateMaterial(RTexture baseColor, RTexture normal, RTexture properties, RMaterial& material);

	/* Uploads data from staging buffer to device. */
	void SubmitAssets();

	/* Executed at start of frame before entity draw events are called. */
	void OnDrawStart();

	/* Executed after entity draw events are called, draws frame. */
	void OnDrawEnd();

	/* Draw static mesh. */
	void DrawStaticMesh(RStaticMesh meshId, RMaterial materialId, mat4 modelTransform);

	/* Set camera. */
	void DrawSetCamera(mat4 transform);

private:
	/**** Option finding utilities. ****/

	uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat FindDepthFormat();


	/**** Initialize/destroy class members. *****/
	void InitDebugMessenger();

	void InitDevice(const std::vector<const char*>& extensions);

	void InitSwapChain();

	void DestroySwapChain();

	void InitDescriptorPool();

	void InitDescriptorSetLayout();

	void InitGraphicsPipeline();

	void InitRenderPass();

	void InitSwapChainFramebuffers();

	void InitDepthbuffer();

	void InitGraphicsCommandPool();

	void InitCommandBuffers();

	void InitSyncObjects();

	void InitDeviceMemoryManager();

	void DestroyDeviceMemoryManager();

	void InitDefaultTextureSampler();

	/* Utility */

	/* Creates new device memory block, returns index. */
	uint32 CreateDeviceMemoryBlock(EMemoryBlockUsage usage);

	/* Allocates memory on device, returns block index and offset to available memory. */
	void DeviceMalloc(VkDeviceSize size, EMemoryBlockUsage usage, VkDeviceSize& offset, uint32& block);

	void AllocateDeviceMemory(VkDeviceSize size, EMemoryLocation location, VkDeviceMemory& deviceMemory);

	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void CreateBuffer(VkDeviceSize size, EMemoryBlockUsage usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);

	void CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size);

	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32 width, uint32 height);

	void CreateTextureMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageMemoryBarrier& barrier);

	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkCommandBuffer BeginSingleTimeCommands();

	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	void GenerateMissingTexture(uint32 width, uint32 tiles, void* textureMemory);

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	std::vector<char> LoadSPIRV(const std::string& filename);

	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);

	void RecreateSwapChain();

	void ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

	IOSystem* pIO;


	VkRenderPass renderPass;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkSampler textureSampler;

	// Cached Draw Commands
	std::vector<SDrawStaticPBR> drawStaticCommands;
	mat4 camera;

	// Sync
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	// Pipelines
	VkDescriptorPool descriptorPool_PBR;
	VkDescriptorSetLayout descriptorSetLayout_PBR;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	// Uniforms
	std::vector<SPushConstants> pushConstants;
	
	// Resource Manager
	std::vector<SMaterialHandle> materials;
	std::vector<SMeshMemoryHandle> meshes;
	std::vector<uint32> meshesInStagingMemory;
	std::vector<STextureMemoryHandle> textures;
	std::vector<uint32> texturesInStagingMemory;

	// Memory Requirements
	VkDeviceSize deviceDataAlignment[4];  /* Data memory-alignment requirement (indexed by EMemoryBlockUsage). */
	uint32 deviceMemoryTypeByLocation[2]; /* Memory type used to allocate a block when indexed by EMemoryLocation. */

	// Memory
	std::vector<SMemoryBlock> deviceMemory;	 /* GPU memory blocks. */
	std::vector<SMemoryBlock> uniformMemory; /* Local-visible GPU memory for changing values. */
	std::vector<SMemoryBlock> stagingMemory; /* Local memory-mapped staging blocks. */
};
