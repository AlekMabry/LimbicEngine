#include "VkUtil.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>

namespace VkUtil
{

/**** General ****/

EResult CreateImageView(
	const SVkContext* context, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& imageView)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(context->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		std::cout << "[ERROR] Failed to create image view!\n";
		return eResult_Error;
	}

	return eResult_Success;
}

/**** Debug/Validation layers. ****/

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void DestroyDebugUtilsMessengerEXT(
	VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			std::cout << "[VERBOSE] " << pCallbackData->pMessage << std::endl;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cout << "[INFO] " << pCallbackData->pMessage << std::endl;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cout << "[WARNING] " << pCallbackData->pMessage << std::endl;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cout << "[ERROR] " << pCallbackData->pMessage << std::endl;
			break;
		default:
			std::cout << "[DEFAULT] " << pCallbackData->pMessage << std::endl;
			break;
	}
	return VK_FALSE;
}

void ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info)
{
	info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
					   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info.pfnUserCallback = DebugCallback;
	info.pUserData = nullptr;
}

void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger)
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	ConfigureDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		std::cout << "[ERROR] Failed to set up debug messenger!\n";
	}
}

/**** Instance creation. ****/

bool CheckInstanceExtensionSupport(std::vector<const char*>& extensions)
{
	uint32 availableExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

	for (uint64 i = 0; i < extensions.size(); i++)
	{
		bool bExtensionFound = false;

		for (const auto& extensionProperties : availableExtensions)
		{
			if (strcmp(extensions[i], extensionProperties.extensionName) == 0)
			{
				bExtensionFound = true;
				break;
			}
		}
		if (!bExtensionFound)
			return false;
	}
	return true;
}

bool CheckValidationLayerSupport(const std::vector<const char*>& layers)
{
	uint32 layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : layers)
	{
		bool bLayerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				bLayerFound = true;
				break;
			}
		}

		if (!bLayerFound)
		{
			return false;
		}
	}

	return true;
}

EResult CreateInstance(VkInstance& instance, const char* applicationName, std::vector<const char*>& extensions, bool bEnableDebug,
	std::vector<const char*>& validationLayers)
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = applicationName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Limbic Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	if (bEnableDebug)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	if (!VkCheckInstanceExtensionSupport(extensions.data(), extensions.size()))
	{
		std::cout << "[ERROR] Required Vulkan instance extensions not available!\n";
		return eResult_Error;
	}

	createInfo.enabledExtensionCount = static_cast<uint32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (bEnableDebug && !VkCheckValidationLayerSupport(validationLayers))
	{
		std::cout << "[WARNING] Vulkan validation layers requested, but not available!\n";
		bEnableDebug = false;
	}

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (bEnableDebug)
	{
		createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		ConfigureDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		std::cout << "[ERROR] Failed to create Vulkan instance!\n";
		return eResult_Error;
	}

	return eResult_Success;
}

/**** Device query. ***/

bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions)
{
	uint32 extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

EResult FindQueueFamilyIndex(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32& queueIndex)
{
	uint32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	/* I can't find any setups in the Vulkan hardware database that don't have a queue family with
	graphics and presentation support other than AMD systems on Linux (which don't have standard presentation
	support anyway). This function and presentation will have to be revaluated if the editor is ever ported to Linux. */
	for (uint32 i = 0; i < queueFamilyCount; i++)
	{
		VkBool32 bPresentSupported = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &bPresentSupported);
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && bPresentSupported)
		{
			queueIndex = i;
			return eResult_Success;
		}
	}

	std::cout << "[ERROR] Unable to find a Vulkan queue family for your GPU with graphics and present support.\n";
	return eResult_Error;
}

SSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SSwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

int32 RatePhysicalDevice(VkPhysicalDevice& physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions)
{
	// Verify that it supports a queue family with required features.
	uint32 queueIndex;
	EResult foundQueueFamily = FindQueueFamilyIndex(physicalDevice, surface, queueIndex);
	if (foundQueueFamily != eResult_Success)
		return 0;

	// Verify extension support.
	if (!VkCheckDeviceExtensionSupport(physicalDevice, extensions))
		return 0;

	// Verify swapchain support
	SSwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice, surface);
	if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
		return 0;

	// Fits minimum requirements
	int32 score = 1;

	// Desired features
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	return score;
}

EResult PickPhysicalDevice(
	VkPhysicalDevice& physicalDevice, VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& extensions)
{
	physicalDevice = VK_NULL_HANDLE;

	uint32 deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		std::cout << "[ERROR] Failed to find GPUs with Vulkan support!\n";
		return eResult_Error;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	std::multimap<int, VkPhysicalDevice> candidates;
	for (auto& device : devices)
	{
		int32 score = RatePhysicalDevice(device, surface, extensions);
		candidates.insert(std::make_pair(score, device));
	}

	if (candidates.rbegin()->first > 0)
	{
		physicalDevice = candidates.rbegin()->second;
		return eResult_Success;
	}
	else
	{
		std::cout << "[ERROR] Unable to find suitable GPU!\n";
		return eResult_Error;
	}
}

/**** Logical device. ****/

EResult CreateDevice(VkDevice& device, VkQueue& queue, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	const std::vector<const char*>& extensions, bool bEnableDebug, const std::vector<const char*>& validationLayers)
{
	uint32 queueIndex;
	FindQueueFamilyIndex(physicalDevice, surface, queueIndex);

	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(extensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

	// For Vulkan support < 1.3 that still makes a distinction between device and validation layers.
	if (bEnableDebug)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32>(validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
	{
		std::cout << "[ERROR] Failed to create logical device!\n";
		return eResult_Error;
	}

	vkGetDeviceQueue(device, queueIndex, 0, &queue);
	return eResult_Success;
}

/**** Swapchain ****/

bool CheckStencilComponentSupport(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int32 width, int32 height)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = {static_cast<uint32>(width), static_cast<uint32>(height)};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height =
			std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

EResult CreateSwapChain(const SVkContext* context, int32 width, int32 height, SVkWindow& window)
{
	SSwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(context->physicalDevice, window.surface);

	VkSurfaceFormatKHR surfaceFormat = PickSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = PickSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = PickSwapExtent(swapChainSupport.capabilities, width, height);

	uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = window.surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = extent;
	swapchainCreateInfo.imageArrayLayers = 1;	 // For VR this may need to be increased
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(context->device, &swapchainCreateInfo, nullptr, &window.swapchain) != VK_SUCCESS)
	{
		std::cout << "[ERROR] Failed to create swap chain!\n";
		return eResult_Error;
	}

	vkGetSwapchainImagesKHR(context->device, window.swapchain, &imageCount, nullptr);
	window.swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(context->device, window.swapchain, &imageCount, window.swapchainImages.data());

	window.swapchainImageFormat = surfaceFormat.format;
	window.swapchainExtent = extent;
	window.swapchainImageViews.resize(imageCount);
	window.swapchainSize = imageCount;

	for (size_t i = 0; i < imageCount; i++)
	{
		CreateImageView(context, window.swapchainImages[i], window.swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
			window.swapchainImageViews[i]);
	}
}

/**** Memory management. ****/

void RenderSystem::PickDeviceMemoryBlockTypes()
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32 memoryLocation = 0; memoryLocation < EMemoryLocationCount; memoryLocation++)
	{
		/* Todo - Sort potential candidates by heap size, smaller sizes are more likely
		to be device-local instead of "virtual" memory. */
		for (uint32 i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			// Assume there is a memory type with the exact flags we need.
			if (memoryProperties.memoryTypes[i].propertyFlags == memoryLocationVkFlags[memoryLocation])
			{
				deviceMemoryTypeByLocation[memoryLocation] = i;
				break;
			}
		}
	}
}

/**** Mega structure creation. ****/

EResult CreateContext(SVkContext& context, std::vector<const char*>& extensions, const char* applicationName, bool bDebugEnabled,
	std::vector<const char*>& validationLayers)
{
	CreateInstance(context.instance, applicationName, extensions, bDebugEnabled, validationLayers);
}

EResult VkCreateWindow(SVkWindow& window, int32 width, int32 height)
{
}

EResult VkResizeWindow(SVkWindow& window)
{
}

}	 // namespace VkUtil