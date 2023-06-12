#pragma once

#include "LimbicTypes.h"

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

#define VK_CHECK(f, message)                                                                            \
	{                                                                                                   \
		VkResult res = (f);                                                                             \
		if (res != VK_SUCCESS)                                                                          \
		{                                                                                               \
			std::cout << "[ERROR] Line: " << __LINE__ << " in " << __FILE__ << ": " << message << "\n"; \
			assert(res == VK_SUCCESS);                                                                  \
		}                                                                                               \
	}

#define LERROR(message) throw std::runtime_error(LSTRINGIFY(message))

struct SVkContext
{
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	uint32 queueFamilyIndex;
	VkQueue queue;
	VkRenderPass renderPass;
	VkCommandPool commandPool;
};

struct SVkWindow
{
	uint32 currentSwapchainImage;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;

	// ---- Frame buffers. ----
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	std::vector<VkFramebuffer> swapchainFramebuffers;

	// ---- Depth buffer. ----
	VkFormat depthImageFormat;
	VkImage depthImage;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;

	// ---- Command buffers. ----
	VkCommandBuffer commandBuffers[2];

	// ---- Sync ----
	VkSemaphore imageAvailableSemaphores[2];
	VkSemaphore renderFinishedSemaphores[2];
	VkFence inFlightFences[2];
};

struct SSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

namespace VkUtil
{
uint32 FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties);

VkFormat FindSupportedFormat(
	VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

void CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& imageView);

void ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger);

//---- Instance creation. ----

bool CheckInstanceExtensionSupport(std::vector<const char*>& extensions);

bool CheckValidationLayerSupport(const std::vector<const char*>& layers);

EResult CreateInstance(const char* applicationName, std::vector<const char*>& extensions, bool bEnableDebug,
	std::vector<const char*>& validationLayers, VkInstance& instance);

//---- Device query. ----

bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);

void FindQueueFamilyIndex(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32& queueIndex);

SSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

int32 RatePhysicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions);

EResult PickPhysicalDevice(
	VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& extensions, VkPhysicalDevice& physicalDevice);

//---- Logical device. ----

EResult CreateDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions,
	bool bEnableDebug, const std::vector<const char*>& validationLayers, VkDevice& device, VkQueue& queue);

void CreateCommandPool(VkDevice device, uint32 queueFamilyIndex, VkCommandPool& commandPool);

//---- Window (swapchain, color/depth targets, framebuffers) ----

bool CheckStencilComponentSupport(VkFormat format);

VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int32 width, int32 height);

void CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, int32 width, int32 height,
	VkSwapchainKHR& swapchain, VkFormat& swapchainImageFormat, VkExtent2D& swapchainExtent);

void CreateSwapchainImages(VkDevice device, VkSwapchainKHR swapchain, VkFormat swapchainImageFormat,
	std::vector<VkImage>& swapchainImages, std::vector<VkImageView>& swapchainImageViews);

void CreateRenderPass(VkDevice device, VkFormat colorImageFormat, VkFormat depthImageFormat, VkRenderPass& renderPass);

void CreateDepthImage(VkDevice device, VkPhysicalDevice physicalDevice, const VkExtent2D& extent, VkFormat& depthImageFormat,
	VkImageView& depthImageView, VkImage& depthImage, VkDeviceMemory& depthImageMemory);

void CreateSwapchainFramebuffers(VkDevice device, VkRenderPass renderPass, const VkExtent2D& extent,
	const std::vector<VkImageView>& swapchainImageViews, VkImageView depthImageView,
	std::vector<VkFramebuffer>& swapchainFramebuffers);

void CreateSemaphores(VkDevice device, VkSemaphore* semaphores, size_t semaphoreCount);

void CreateSignaledFences(VkDevice device, VkFence* fences, size_t fenceCount);


}	 // namespace VkUtil