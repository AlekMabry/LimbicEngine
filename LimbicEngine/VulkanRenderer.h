#pragma once

#include "LimbicTypes.h"

#include <Windows.h>

#include <cstring>
#include <iostream>
#include <optional>
#include <set>
#include <cstdint>
#include <vector>
#include <fstream>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>

//#define GLM_FORCE_LEFT_HANDED
//#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

using namespace glm;

#ifdef NDEBUG
const bool bEnableValidationLayers = false;
#else
const bool bEnableValidationLayers = true;
const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
#endif

const uint32 MAX_FRAMES_IN_FLIGHT = 2;

struct SUniformBufferObject
{
	mat4 transform;
};

struct SQueueFamilyIndices
{
	std::optional<uint32> graphicsFamily;
	std::optional<uint32> presentFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

enum ETextureFormat
{
	eTextureFormatRGBA,
	eTextureFormatDXT1
};

const VkFormat textureFormatVkFormat[2] = {VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_BC1_RGB_UNORM_BLOCK};

struct SDrawPBRMesh
{
	uint32 meshIndex;
	uint32 albedoTextureIndex;
	uint32 normalTextureIndex;
	uint32 materialTextureIndex;
	uint32 lightmapTextureIndex;
	mat4 transform;
};

struct SLights
{
	vec4 lights[8];
};

enum EMemoryLocation
{
	eMemoryLocationHost,
	eMemoryLocationDevice
};

/* Maps EMemoryLocation enums to the Vulkan memory property flags. */
const VkMemoryType memoryLocationVkFlags[2] = {VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

enum EMemoryBlockUsage
{
	eMemoryBlockUsageGeometry,
	eMemoryBlockUsageUniforms,
	eMemoryBlockUsageImages,
	eMemoryBlockUsageStaging
};

/* Maps EMemoryBlockUsage enums to the Vulkan buffer usage flags. */
const VkBufferUsageFlags memoryBlockUsageVkFlags[4] = {
	VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT
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

struct STextureMemoryHandle
{
	EMemoryLocation location;
	uint32 block;
	VkDeviceSize offset;
	VkDeviceSize size;
	VkImage image;
};

struct SMemoryBlock
{
	EMemoryBlockUsage usage;
	VkBuffer buffer;
	VkImage image;
	VkDeviceMemory memory;
	VkDeviceSize cursor;
	VkDeviceSize availableMemory;
	void* mappedLocation;
};

const VkDeviceSize MEMORY_BLOCK_SIZE = 32 * 1024;

class VulkanRenderer
{
public:
	VulkanRenderer(const char* applicationName, uint32 width, uint32 height, HWND window, HINSTANCE process);

	~VulkanRenderer();
	
	/* Call to tell renderer that the window surface has been resized. */
	void Resize();

	/* Creates static mesh, provides staging buffer to write vertices and indices to. */
	void CreateStaticMesh(uint32 vertices, uint32 indices, uint32& meshHandle, SStaticVertex*& vertexBuffer, uint32*& indexBuffer);

	/* Creates texture, provides staging buffer to write image to. */
	void CreateTexture(uint32 width, uint32 height, ETextureFormat format, uint32& textureHandle, void*& textureBuffer);

	/* Uploads data from staging buffer to device. */
	void SubmitAssets();

	/* Flags object for deletion. */
	void DeleteObject(uint32 objectIndex);

	/* Set lights for current frame. */
	void FrameSetLights(SLights* lights);

	/* Set static PBR mesh uniforms for current frame. */
	void FrameSetStaticPBRMeshes(uint32 meshCount, SDrawPBRMesh* meshes);

	/* Draw current frame. */
	void FrameDraw();

private:
	/**** QUERYING/PICKING UTILITIES ****/
	bool CheckInstanceExtensionSupport(const std::vector<const char*>& extensions);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	bool CheckValidationLayerSupport(const std::vector<const char*>& layers);
	bool IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	void PickPhysicalDevice(const std::vector<const char*>& extensions);
	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);
	void PickDeviceMemoryHeaps();
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	std::vector<char> LoadSPIRV(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);

	/**** INITIALIZE *****/
	void InitInstance();
	void InitDebugMessenger();
	void InitDevice(const std::vector<const char*>& extensions);
	void InitSurface();
	void InitSwapChain();
	void InitGraphicsPipeline();
	void InitRenderPass();
	void InitSwapChainFramebuffers();
	void InitGraphicsCommandPool();
	void InitCommandBuffers();
	void InitSyncObjects();
	

	void RecreateSwapChain();
	void DestroySwapChain();
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size);
	void CreateDescriptorSetLayout();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void CreateTextureImage();
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32 width, uint32 height);
	void GenerateMissingTexture(uint32 width, uint32 tiles, void* textureMemory);
	void CreateTextureImageView();
	void CreateTextureSampler();
	void CreateDepthResources();

	/**** DESTROY ****/

	/**** DYNAMIC ****/

	/**** MEMORY MANAGER ***/

	void CreateDeviceMemoryManager();
	void DestroyDeviceMemoryManager();

	/* Creates new device memory block, returns index. */
	uint32 CreateDeviceMemoryBlock(EMemoryBlockUsage usage);

	/* Allocates memory on device, returns block index and offset to available memory. */
	void DeviceMalloc(VkDeviceSize size, EMemoryBlockUsage usage, VkDeviceSize& offset, uint32& block);

	/**** DEBUG ****/
	void ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

	HWND win32Window;
	HINSTANCE win32Process;
	HANDLE win32Console;

	uint32 currentFrame;
	uint32 width;
	uint32 height;
	std::string applicationName;

	VkInstance instance;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkImage depthImage;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkImageView depthImageView;
	VkSampler textureSampler;
	VkDeviceMemory depthImageMemory;
	bool framebufferResized = false;

	VkDeviceSize deviceDataAlignment[4];	/* Data memory-alignment requirement (indexed by EMemoryBlockUsage). */
	uint32 memoryBlockTypeIndex[4];		 /* Memory type used to allocate a block when indexed by EMemoryBlockUsage. */	
	uint32 memoryBlockHeapIndex[4];		 /* Memory heap used to allocate a block when indexed by EMemoryBlockUsage.*/

	std::vector<SMeshMemoryHandle> meshes;
	std::vector<uint32> meshesInStagingMemory;
	std::vector<STextureMemoryHandle> textures;
	std::vector<uint32> texturesInStagingMemory;

	std::vector<SMemoryBlock> deviceMemory;		/* GPU memory blocks. */
	std::vector<SMemoryBlock> uniformMemory;	/* Local-visible GPU memory for changing values. */
	std::vector<SMemoryBlock> stagingMemory;	/* Local memory-mapped staging blocks. */
};
