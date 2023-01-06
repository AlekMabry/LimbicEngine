#include "VkUtil.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>

namespace VkUtil
{

//---- General ----

/**	Finds a memory type index (see VkMemoryType array in VkPhysicalDeviceMemoryProperties).
	@param[in]	physicalDevice	Valid physical device.
	@param[in]	typeFilter
	@param[in]	properties		Memory property flags (ie. DEVICE_LOCAL_BIT, HOST_VISIBLE_BIT, etc) */
uint32 FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32 i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	LERROR("[ERROR] Failed to find suitable memory type!");
}

VkFormat FindSupportedFormat(
	VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	LERROR("Failed to find supported format!");
}

void CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& imageView)
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

	VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &imageView), "Failed to create image view!")
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

EResult CreateInstance(const char* applicationName, std::vector<const char*>& extensions, bool bEnableDebug,
	std::vector<const char*>& validationLayers, VkInstance& instance)
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

void FindQueueFamilyIndex(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32& queueIndex)
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
			return;
		}
	}

	LERROR("[ERROR] Unable to find a Vulkan queue family for your GPU with graphics and present support.\n");
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

int32 RatePhysicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions)
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
	VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& extensions, VkPhysicalDevice& physicalDevice)
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

EResult CreateDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char*>& extensions,
	bool bEnableDebug, const std::vector<const char*>& validationLayers, VkDevice& device, VkQueue& queue)
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

void CreateCommandPool(VkDevice device, uint32 queueFamilyIndex, VkCommandPool& commandPool)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndex;

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create command pool!");
	}
}

/**** Window ****/

/** Checks if the given image format supports a stencil component. */
bool CheckStencilComponentSupport(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

/**	Picks the swapchain surface format from a list of options. Prefers SRGB nonlinear color. */
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

/**	Picks the present mode from a list of options. Prefers Mailbox but will fallback to FIFO (VSync). */
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

/**	Picks the size of the swapchain framebuffer. Attempts to use the given width and height, but will clamp to what the hardware
	supports. */
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

/**	Chooses the image format and size of the framebuffer images, creates swapchain.
	@param[in]	device					Valid logical device.
	@param[in]	physicalDevice			Valid physical device.
	@param[in]	surface					Valid surface.
	@param[in]	width					Preferred framebuffer width (should match surface size, but will be clamped to hardware).
	@param[in]	height					Preferred framebuffer height (should match surface size, but will be clamped to hardware).
	@param[out]	swapchain				Swapchain to create.
	@param[out]	swapchainImageFormat	Selected device-supported swapchain framebuffer format.
	@param[out]	swapchainExtent			Selected device-supported swapchain framebuffer size. */
void CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, int32 width, int32 height,
	VkSwapchainKHR& swapchain, VkFormat& swapchainImageFormat, VkExtent2D& swapchainExtent)
{
	SSwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice, surface);

	VkSurfaceFormatKHR surfaceFormat = PickSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = PickSwapPresentMode(swapChainSupport.presentModes);
	swapchainExtent = PickSwapExtent(swapChainSupport.capabilities, width, height);

	uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchainExtent;
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

	swapchainImageFormat = surfaceFormat.format;

	VK_CHECK(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain), "Failed to create swapchain!")
}

/**	Loads swapchain images into the VkImages vector and generates VkImageViews for them.
	@param[in]	device				Valid logical device.
	@param[in]	swapchain			Valid swapchain.
	@param[out]	swapchainImages		Vector of VkImages generated by the swapchain.
	@param[out]	swapchainImageViews	Vector of VkImageViews generated from swapchain.
*/
void CreateSwapchainImages(VkDevice device, VkSwapchainKHR swapchain, VkFormat swapchainImageFormat,
	std::vector<VkImage>& swapchainImages, std::vector<VkImageView>& swapchainImageViews)
{
	uint32 imageCount;
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

	for (size_t i = 0; i < imageCount; i++)
	{
		CreateImageView(device, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, swapchainImageViews[i]);
	}
}

/** Creates a render pass.
	@param[in]	device				Valid logical device.
	@param[in]	colorImageFormat	Valid color target image format.
	@param[in]	depthImageFormat	Valid depth target image format.
	@param[out]	renderPass			Created renderpass. */
void CreateRenderPass(VkDevice device, VkFormat colorImageFormat, VkFormat depthImageFormat, VkRenderPass& renderPass)
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = colorImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;	 // Clear framebuffer
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = depthImageFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments = attachments;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_CHECK(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass), "Failed to create render pass!")
}

/**	Selects a supported image format, creates, and allocates a depth image.
	@param[in]	device				Valid logical device.
	@param[in]	physicalDevice		Valid physical device.
	@param[in]	extent				Valid extent (should be same as color target).
	@param[out]	depthImageFormat	Output depth image format.
	@param[out]	depthImageView		Output depth image view.
	@param[out]	depthImage			Output depth image.
	@param[out]	depthImageMemory	Output depth image memory. */
void CreateDepthImage(VkDevice device, VkPhysicalDevice physicalDevice, const VkExtent2D& extent, VkFormat& depthImageFormat,
	VkImageView& depthImageView, VkImage& depthImage, VkDeviceMemory& depthImageMemory)
{
	depthImageFormat =
		FindSupportedFormat(physicalDevice, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = extent.width;
	imageInfo.extent.height = extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = depthImageFormat;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &depthImage), "Failed to create depth image!")

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, depthImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &depthImageMemory), "Failed to allocate depth image memory!")

	vkBindImageMemory(device, depthImage, depthImageMemory, 0);

	CreateImageView(device, depthImage, depthImageFormat, VK_IMAGE_ASPECT_DEPTH_BIT, depthImageView);
}

/**	Creates a vector of swapchain framebuffers given color target image views, extent, and a depth target image view. */
void CreateSwapchainFramebuffers(VkDevice device, VkRenderPass renderPass, const VkExtent2D& extent,
	const std::vector<VkImageView>& swapchainImageViews, VkImageView depthImageView,
	std::vector<VkFramebuffer>& swapchainFramebuffers)
{
	swapchainFramebuffers.resize(swapchainImageViews.size());

	for (size_t i = 0; i < swapchainImageViews.size(); i++)
	{
		VkImageView attachments[2] = {swapchainImageViews[i], depthImageView};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 2;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]), "Failed to create framebuffer!")
	}
}

EResult CreateSyncObjects(VkDevice device, SVkWindow* window)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < 2; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &window->imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &window->renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &window->inFlightFences[i]) != VK_SUCCESS)
		{
			std::cout << "[ERROR] Failed to create sync objects for a frame!\n";
			return eResult_Error;
		}
	}
	return eResult_Success;
}

void CreateSemaphores(VkDevice device, VkSemaphore* semaphores, size_t semaphoreCount)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for (size_t i = 0; i < semaphoreCount; i++)
	{
		VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphores[i]), "Failed to create semaphores!");
	}
}

void CreateSignaledFences(VkDevice device, VkFence* fences, size_t fenceCount)
{
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < fenceCount; i++)
	{
		VK_CHECK(vkCreateFence(device, &fenceInfo, nullptr, &fences[i]), "Failed to create fences!");
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