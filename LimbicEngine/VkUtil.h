#pragma once

#include "LimbicTypes.h"
#include <vulkan/vulkan.h>
#include <optional>

#define VK_CHECK(f)                                                 \
	{                                                               \
		VkResult res = (f);                                         \
		if (res != VK_SUCCESS)                                      \
		{                                                           \
			std::cout << "[ERROR] Line: " << __LINE__ << std::endl; \
			assert(res == VK_SUCCESS);                              \
		}                                                           \
	}

#define MAX_SWAPCHAIN_FRAMES	2

struct SVkContext
{
	SVkContext();

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};

struct SVkWindow
{
	uint32 currentSwapchainFrame;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;
	// ---- Frame buffers. ----
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;
	VkImage swapchainImages[MAX_SWAPCHAIN_FRAMES];
	VkImageView swapchainImageViews[MAX_SWAPCHAIN_FRAMES];
	VkFramebuffer swapchainFramebuffers[MAX_SWAPCHAIN_FRAMES];
	// ---- Depth buffer. ----
	VkImage depthImage;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;
	// ---- Command buffers. ----

	// ---- Sync ----
	VkSemaphore imageAvailableSemaphores[MAX_SWAPCHAIN_FRAMES];
	VkSemaphore renderFinishedSemaphores[MAX_SWAPCHAIN_FRAMES];
	VkFence inFlightFences[MAX_SWAPCHAIN_FRAMES];
};

struct SSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

const uint32 MAX_SWAPCHAIN_SIZE = 2;

/**** Instance creation. ****/

bool VkCheckInstanceExtensionSupport(const char** extensions, uint64 extensionCount);

bool VkCheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);

bool VkCheckValidationLayerSupport(const std::vector<const char*>& layers);

EResult VkCreateInstance(VkInstance& instance, const char* applicationName, const char** extensions, uint64 extensionCount, bool bEnableDebug);

/**** Device query. ****/

