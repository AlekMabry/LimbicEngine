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

enum ETextureType
{
	TEXTURE_TYPE_DXT1
};

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
	MEMORY_LOCATION_HOST,
	MEMORY_LOCATION_SHARED,
	MEMORY_LOCATION_DEVICE
};

struct SMeshMemoryIndex
{
	EMemoryLocation location;
	uint32 vertexBlock;
	uint32 indexBlock;
	VkDeviceSize vertexOffset;
	uint32 vertices;
	VkDeviceSize indexOffset;
	uint32 indices;
};

struct SMemoryBlock
{
	VkBuffer buffer;
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
	void CreateStaticMesh(uint32 vertices, uint32 indices, uint32& meshIndex, SStaticVertex*& vertexBuffer, uint32*& indexBuffer);

	/* Uploads data from staging buffer to device. */
	void SubmitStaticMeshes();

	/* Creates texture, provides staging buffer to write image to. */
	void CreateTexture(uint32 width, uint32 height, ETextureType type, uint32& objectIndexIndex, void** textureBuffer);

	/* Uploads data from texture buffer to device. */
	void SubmitTexture(uint32 objectIndex);

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

	/**** CREATE *****/
	void CreateInstance();
	void CreateDebugMessenger();
	void CreateLogicalDevice(const std::vector<const char*>& extensions);
	void CreateSurface();
	void CreateSwapChain();
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateImageViews();
	void CreateGraphicsPipeline();
	std::vector<char> LoadSPIRV(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);
	void CreateSyncObjects();
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
	void DeviceMalloc(VkDeviceSize size, uint32 alignment, size_t& offset, uint32& block);

	/**** DEBUG ****/
	void ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
	
	/*
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	*/

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
	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;
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

	VkDeviceSize deviceVertexAlignment;
	VkDeviceSize deviceIndexAlignment;

	std::vector<SMeshMemoryIndex> meshes;
	std::vector<uint32> meshesInStagingMemory;

	std::vector<SMemoryBlock> deviceMemory;		/* GPU memory blocks. */
	std::vector<SMemoryBlock> uniformMemory;	/* Local-visible GPU memory for changing values. */
	std::vector<SMemoryBlock> stagingMemory;	/* Local memory-mapped staging blocks. */
};
