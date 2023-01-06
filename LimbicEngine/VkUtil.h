#pragma once

#include "LimbicTypes.h"

#include <vulkan/vulkan.h>

#include <optional>

#define VK_CHECK(f, message)                                                                                        \
	{                                                                                                               \
		VkResult res = (f);                                                                                         \
		if (res != VK_SUCCESS)                                                                                      \
		{                                                                                                           \
			std::cout << "[ERROR] Line: " << __LINE__ << " in " << __FILE__ << ": " << message << "\n"; \
			assert(res == VK_SUCCESS);                                                                              \
		}                                                                                                           \
	}

#define LERROR(message) throw std::runtime_error(LSTRINGIFY(message))

#define MAX_SWAPCHAIN_FRAMES 2

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

/**** Instance creation. ****/

bool VkCheckInstanceExtensionSupport(const char** extensions, uint64 extensionCount);

bool VkCheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);

bool VkCheckValidationLayerSupport(const std::vector<const char*>& layers);

EResult VkCreateInstance(
	VkInstance& instance, const char* applicationName, const char** extensions, uint64 extensionCount, bool bEnableDebug);

/**** Device query. ****/
