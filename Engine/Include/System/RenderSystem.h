#pragma once

#include <LimbicTypes.h>

#include <Windows.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>

#include <map>

#ifdef NDEBUG
const bool bEnableValidationLayers = false;
const std::vector<const char*> validationLayers = {};
#else
class RWindow;
const bool bEnableValidationLayers = true;
const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
#endif

#define VK_CHECK(f)                                                 \
	{                                                               \
		VkResult res = (f);                                         \
		if (res != VK_SUCCESS)                                      \
		{                                                           \
			std::cout << "[ERROR] Line: " << __LINE__ << std::endl; \
			assert(res == VK_SUCCESS);                              \
		}                                                           \
	}

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

/* Supported texture formats. */
enum ETextureFormat
{
	eTextureFormatRGBA,
	eTextureFormatDXT1
};

const uint32 ETextureFormatCount = 2;

/* Maps ETextureFormat enums to the Vulkan image formats. */
const VkFormat textureFormatVkFormat[2] = {VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_BC1_RGB_UNORM_BLOCK};

enum EMemoryLocation
{
	eMemoryLocationHost,
	eMemoryLocationDevice
};

const uint32 EMemoryLocationCount = 2;

/* Maps EMemoryLocation enums to the Vulkan memory property flags. */
const VkMemoryPropertyFlags memoryLocationVkFlags[2] = {
	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT};

enum EMemoryBlockUsage
{
	eMemoryBlockUsageGeometry,
	eMemoryBlockUsageUniforms,
	eMemoryBlockUsageImages,
	eMemoryBlockUsageStaging
};

const uint32 EMemoryBlockUsageCount = 4;

/* Maps EMemoryBlockUsage enums to the Vulkan buffer usage flags. */
const VkBufferUsageFlags memoryBlockUsageVkFlags[4] = {
	VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
	VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT};

/* Mape EMemoryBlockUsage enums to EMemoryLocation enums. */
const EMemoryLocation memoryBlockUsageLocation[4] = {
	eMemoryLocationDevice, eMemoryLocationHost, eMemoryLocationDevice, eMemoryLocationHost};

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

const VkDeviceSize MEMORY_BLOCK_SIZE = 8 * 1024 * 2048;

class RenderSystem
{
	friend class RView;
	friend class RWindow_GLFW;

public:
	~RenderSystem();

	/* Initializes vk instance, next step is to add a window. */
	VkInstance InitInstance(const char* applicationName);

	/* After window creation, configures device and queues. */
	VkInstance InitGLFW();

	/* Configure from Qt. */
	void InitQt(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue,
		VkQueue presentQueue, VkCommandPool commandPool);

	/* After InitGLFW or QtVulkanWindow created. */
	void Init();

	/* Each window/view pair manages a surface and it's draw settings. */
	void AddWindow(const std::string& name, RWindow &window);

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

	/* Get window and view. */
	std::pair<RWindow*, RView*> GetWindowView(const std::string& window);

private:
	/**** Support checking utilities. ****/

	bool CheckInstanceExtensionSupport(const std::vector<const char*>& extensions);

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);

	bool CheckValidationLayerSupport(const std::vector<const char*>& layers);

	bool CheckStencilComponentSupport(VkFormat format);

	bool IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions);

	/**** Option finding utilities. ****/

	/* A window surface must be initialized first. */
	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat FindDepthFormat();

	/**** Option picking utilities. ****/
	void PickPhysicalDevice(const std::vector<const char*>& extensions);

	void PickDeviceMemoryBlockTypes();

private:
	void InitDebugMessenger();

	void InitDevice(const std::vector<const char*>& extensions);

	void InitDeviceMemoryManager();

	void DestroyDeviceMemoryManager();

	/* Utility */

	/* Creates new device memory block, returns index. */
	uint32 CreateDeviceMemoryBlock(EMemoryBlockUsage usage);

	/* Allocates memory on device, returns block index and offset to available memory. */
	void DeviceMalloc(VkDeviceSize size, EMemoryBlockUsage usage, VkDeviceSize& offset, uint32& block);

	void AllocateDeviceMemory(VkDeviceSize size, EMemoryLocation location, VkDeviceMemory& deviceMemory);

	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void CreateBuffer(VkDeviceSize size, EMemoryBlockUsage usage, VkBuffer& buffer,
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

	void ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

	
	std::string applicationName;
	HANDLE win32Console;

	VkInstance instance;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkCommandPool commandPool;

	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice;

	// Cached Draw Commands
	std::vector<SDrawStaticPBR> drawStaticCommands;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;


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

	std::map<std::string, RWindow*> windows;
};
