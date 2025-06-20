#include "Entity/EEntity.h"
#include "Entity/EEntity.h"
#include "Renderer/RWindow_GLFW.h"
#include "Renderer/RView.h"

#include <System/RenderSystem.h>
#include <Renderer/RWindow.h>

/**** Static debug callback configuration. ****/

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

/*
static void DestroyDebugUtilsMessengerEXT(
	VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}
*/

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	HANDLE win32Console = static_cast<HANDLE>(pUserData);

	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			SetConsoleTextAttribute(win32Console, FOREGROUND_GREEN);
			std::cout << "[VERBOSE] " << pCallbackData->pMessage << std::endl;
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			SetConsoleTextAttribute(win32Console, FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << "[INFO] " << pCallbackData->pMessage << std::endl;
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			SetConsoleTextAttribute(win32Console, FOREGROUND_GREEN | FOREGROUND_RED);
			std::cout << "[WARNING] " << pCallbackData->pMessage << std::endl;
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED);
			std::cout << "[ERROR] " << pCallbackData->pMessage << std::endl;
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		default:
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED);
			std::cout << "[DEFAULT] " << pCallbackData->pMessage << std::endl;
			SetConsoleTextAttribute(win32Console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
	}
	return VK_FALSE;
}

/**** Public interface. ****/

RenderSystem::~RenderSystem()
{
	/*
	DestroySwapChain();

	vkDestroySampler(device, textureSampler, nullptr);
	vkDestroyDescriptorPool(device, descriptorPool_PBR, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout_PBR, nullptr);

	vkDestroyPipeline(device, pipeline_graphics, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout_graphics, nullptr);

	vkDestroyRenderPass(device, renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(device, commandPool, nullptr);

	if (bEnableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
	*/
}

VkInstance RenderSystem::InitInstance(const char* applicationName)
{
	this->applicationName = applicationName;
	win32Console = GetStdHandle(STD_OUTPUT_HANDLE);

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = this->applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Limbic Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

	if (bEnableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	if (!CheckInstanceExtensionSupport(extensions))
		throw std::runtime_error("[ERROR] Required instance extensions not available!");

	createInfo.enabledExtensionCount = static_cast<uint32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (bEnableValidationLayers && !CheckValidationLayerSupport(validationLayers))
		throw std::runtime_error("[ERROR] Validation layers requested, but not available!");

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (bEnableValidationLayers)
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
		throw std::runtime_error("[ERROR] Failed to create Vulkan instance!");
	}

	return nullptr;
}

VkInstance RenderSystem::InitGLFW()
{
	std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	PickPhysicalDevice(deviceExtensions);
	InitDevice(deviceExtensions);
	PickDeviceMemoryBlockTypes();
	return instance;
}

void RenderSystem::InitQt(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkQueue presentQueue, VkCommandPool commandPool)
{
	this->device = device;
	this->physicalDevice = physicalDevice;
	this->graphicsQueue = graphicsQueue;
	this->presentQueue = presentQueue;
	this->commandPool = commandPool;
}

void RenderSystem::Init()
{
	PickDeviceMemoryBlockTypes();
	InitDeviceMemoryManager();
}

void RenderSystem::AddWindow(const std::string& name, RWindow& window)
{
	windows.insert({name, &window});
}

void RenderSystem::CreateStaticMesh(
	uint32 vertices, uint32 indices, uint32& meshHandle, SStaticVertex*& vertexBuffer, uint32*& indexBuffer)
{
	SMeshMemoryHandle mesh;
	mesh.location = eMemoryLocationHost;
	VkDeviceSize vertexSize = (vertices * sizeof(SStaticVertex));
	VkDeviceSize indexSize = (indices * sizeof(uint32));

	// Todo - pick a block with enough memory.
	SMemoryBlock* stage = &stagingMemory[0];
	mesh.vertexBlock = 0;
	mesh.indexBlock = 0;

	mesh.vertexOffset = stage->cursor;
	mesh.vertices = vertices;
	vertexBuffer = reinterpret_cast<SStaticVertex*>((uint8*) stage->mappedLocation + stage->cursor);
	stage->cursor += vertexSize;

	mesh.indexOffset = stage->cursor;
	mesh.indices = indices;
	indexBuffer = reinterpret_cast<uint32*>((uint8*) stage->mappedLocation + stage->cursor);
	stage->cursor += indexSize;

	meshHandle = static_cast<uint32>(meshes.size());
	meshesInStagingMemory.push_back(static_cast<uint32>(meshes.size()));
	meshes.push_back(mesh);
}

void RenderSystem::CreateTexture(uint32 width, uint32 height, ETextureFormat format, uint32& textureHandle, void*& textureBuffer)
{
	STextureMemoryHandle texture = {};
	texture.width = width;
	texture.height = height;
	texture.format = format;
	switch (format)
	{
		case eTextureFormatRGBA:
			texture.size = static_cast<uint64>(width) * static_cast<uint64>(height) * 4;
			break;
		case eTextureFormatDXT1:
			texture.size = (width * height) / 2;
			break;
	}

	// Todo - pick a block with enough memory.
	SMemoryBlock* stage = &stagingMemory[1];
	texture.block = 1;
	texture.location = eMemoryLocationHost;

	texture.offset = stage->cursor;
	textureBuffer = reinterpret_cast<void*>((uint8*) stage->mappedLocation + stage->cursor);
	stage->cursor += texture.size;

	texturesInStagingMemory.push_back(static_cast<uint32>(textures.size()));
	textureHandle = static_cast<uint32>(textures.size());
	textures.push_back(texture);
}

void RenderSystem::CreateMaterial(RTexture baseColor, RTexture normal, RTexture properties, RMaterial& material)
{
	if (windows.empty())
	{
		throw std::runtime_error("[ERROR] Attempted to create material before window was created.");
	}
	auto pV = windows.begin()->second->GetRenderView();

	SMaterialHandle mat;
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pV->descriptorPool_PBR;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &pV->descriptorSetLayout_PBR;

	VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, &mat.descriptor));

	VkDescriptorImageInfo baseColorInfo{};
	baseColorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	baseColorInfo.imageView = textures[baseColor].imageView;
	baseColorInfo.sampler = pV->textureSampler;

	VkDescriptorImageInfo normalInfo{};
	normalInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	normalInfo.imageView = textures[normal].imageView;
	normalInfo.sampler = pV->textureSampler;

	VkDescriptorImageInfo propertiesInfo{};
	propertiesInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	propertiesInfo.imageView = textures[properties].imageView;
	propertiesInfo.sampler = pV->textureSampler;

	std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = mat.descriptor;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &baseColorInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = mat.descriptor;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &normalInfo;

	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[2].dstSet = mat.descriptor;
	descriptorWrites[2].dstBinding = 2;
	descriptorWrites[2].dstArrayElement = 0;
	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[2].descriptorCount = 1;
	descriptorWrites[2].pImageInfo = &propertiesInfo;

	vkUpdateDescriptorSets(device, static_cast<uint32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	material = static_cast<RMaterial>(materials.size());
	materials.push_back(mat);
}

void RenderSystem::SubmitAssets()
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	for (uint32 meshIndex : meshesInStagingMemory)
	{
		VkBufferCopy copyVertexRegion, copyIndexRegion;
		uint32 vertexBlock, indexBlock;

		copyVertexRegion.size = meshes[meshIndex].vertices * sizeof(SStaticVertex);
		copyVertexRegion.srcOffset = meshes[meshIndex].vertexOffset;
		DeviceMalloc(copyVertexRegion.size, eMemoryBlockUsageGeometry, copyVertexRegion.dstOffset, vertexBlock);
		vkCmdCopyBuffer(commandBuffer, stagingMemory[meshes[meshIndex].vertexBlock].buffer, deviceMemory[vertexBlock].buffer, 1,
			&copyVertexRegion);

		copyIndexRegion.size = meshes[meshIndex].indices * sizeof(uint32);
		copyIndexRegion.srcOffset = meshes[meshIndex].indexOffset;
		DeviceMalloc(copyIndexRegion.size, eMemoryBlockUsageGeometry, copyIndexRegion.dstOffset, indexBlock);
		vkCmdCopyBuffer(commandBuffer, stagingMemory[meshes[meshIndex].indexBlock].buffer, deviceMemory[indexBlock].buffer, 1,
			&copyIndexRegion);

		// Update mesh indexing to new device memory location
		meshes[meshIndex].location = eMemoryLocationHost;
		meshes[meshIndex].vertexBlock = vertexBlock;
		meshes[meshIndex].indexBlock = indexBlock;
		meshes[meshIndex].vertexOffset = copyVertexRegion.dstOffset;
		meshes[meshIndex].indexOffset = copyIndexRegion.dstOffset;
	}

	for (uint32 textureIndex : texturesInStagingMemory)
	{
		STextureMemoryHandle* texture = &textures[textureIndex];
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = texture->width;
		imageInfo.extent.height = texture->height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = textureFormatVkFormat[texture->format];
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = memoryBlockUsageVkFlags[eMemoryBlockUsageImages];
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &texture->image));
		
		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(device, texture->image, &memoryRequirements);
		deviceDataAlignment[eMemoryBlockUsageImages] = memoryRequirements.alignment;	// This should be passed to the DeviceMalloc

		VkImageMemoryBarrier stagingBarrier{}, shaderBarrier{};
		CreateTextureMemoryBarrier(
			texture->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, stagingBarrier);
		CreateTextureMemoryBarrier(texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, shaderBarrier);

		VkDeviceSize imageDstOffset;
		uint32 deviceBlock;
		DeviceMalloc(memoryRequirements.size, eMemoryBlockUsageImages, imageDstOffset, deviceBlock);
		vkBindImageMemory(device, texture->image, deviceMemory[deviceBlock].memory, imageDstOffset);

		texture->imageView = CreateImageView(texture->image, textureFormatVkFormat[texture->format], VK_IMAGE_ASPECT_COLOR_BIT);

		VkBufferImageCopy region{};
		region.bufferOffset = texture->offset;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {texture->width, texture->height, 1};

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
			nullptr, 1, &stagingBarrier);
		vkCmdCopyBufferToImage(
			commandBuffer, stagingMemory[texture->block].buffer, texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
			nullptr, 1, &shaderBarrier);

		// Update texture indexing to new device memory location
		texture->location = eMemoryLocationDevice;
		texture->offset = 0;
		texture->size = memoryRequirements.size;
		texture->block = deviceBlock;
	}

	EndSingleTimeCommands(commandBuffer);
	meshesInStagingMemory.clear();
	texturesInStagingMemory.clear();
	stagingMemory[0].cursor = 0;
	stagingMemory[1].cursor = 0;
}

void RenderSystem::OnDrawStart()
{
	drawStaticCommands.clear();
}

void RenderSystem::OnDrawEnd()
{
	for (auto& window : windows)
	{
		window.second->RequestUpdate();
	}
}

void RenderSystem::DrawStaticMesh(RStaticMesh meshId, RMaterial materialId, mat4 modelTransform, bool selected)
{
	SDrawStaticPBR drawCommand = {meshId, materialId, modelTransform, selected};
	drawStaticCommands.push_back(drawCommand);
}

std::pair<RWindow*, RView*> RenderSystem::GetWindowView(const std::string& window)
{
	if (windows.contains(window))
	{
		auto pW = windows[window];
		return {pW, pW->GetRenderView()};
	}
	return {};
}

/**** Support checking utilities. ****/

bool RenderSystem::CheckInstanceExtensionSupport(const std::vector<const char*>& extensions)
{
	uint32 availableExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

	for (const char* extensionName : extensions)
	{
		bool bExtensionFound = false;

		for (const auto& extensionProperties : availableExtensions)
		{
			if (strcmp(extensionName, extensionProperties.extensionName) == 0)
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

bool RenderSystem::CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions)
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

bool RenderSystem::CheckValidationLayerSupport(const std::vector<const char*>& layers)
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

bool RenderSystem::CheckStencilComponentSupport(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

bool RenderSystem::IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions)
{
	SQueueFamilyIndices indices = FindQueueFamilies(device);

	bool bExtensionsSupported = CheckDeviceExtensionSupport(device, extensions);

	if (windows.empty())
	{
		throw std::runtime_error("[ERROR] Vulkan attempted to query swap chain support before a window was created.");
	}
	auto pW = windows.begin()->second;

	bool bSwapChainAcceptable = false;
	if (bExtensionsSupported)
	{
		SSwapChainSupportDetails swapChainSupport = dynamic_cast<RWindow_GLFW*>(pW)->QuerySwapChainSupport(device);
		bSwapChainAcceptable = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	// VkPhysicalDeviceFeatures supportedFeatures;
	// vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.IsComplete() && bExtensionsSupported && bSwapChainAcceptable /* && supportedFeatures.samplerAnisotropy */;
}

/**** Option finding utilities. ****/

SQueueFamilyIndices RenderSystem::FindQueueFamilies(VkPhysicalDevice device)
{
	SQueueFamilyIndices indices;

	uint32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	if (windows.empty())
	{
		throw std::runtime_error("[ERROR] Vulkan attempted to find queue families before a surface was initialized.");
	}
	auto surface = windows.begin()->second->GetSurface();

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

uint32 RenderSystem::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties)
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

	throw std::runtime_error("[ERROR] Failed to find suitable memory type!");
}

VkFormat RenderSystem::FindSupportedFormat(
	const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
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

	throw std::runtime_error("[ERROR] Failed to find supported format!");
}

VkFormat RenderSystem::FindDepthFormat()
{
	return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
		VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

/**** Option picking utilities. ****/

void RenderSystem::PickPhysicalDevice(const std::vector<const char*>& extensions)
{
	physicalDevice = VK_NULL_HANDLE;

	uint32 deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("[ERROR] Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device, extensions))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("[ERROR] Failed to find a suitable GPU!");
	}
}

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

/**** Initialize/destroy class members. *****/

void RenderSystem::InitDebugMessenger()
{
	if (!bEnableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	ConfigureDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to set up debug messenger!");
	}
}

void RenderSystem::InitDevice(const std::vector<const char*>& extensions)
{
	SQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32 queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(extensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

	// For Vulkan support < 1.3 that still makes a distrinction between device and validation layers.
	if (bEnableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void RenderSystem::InitDeviceMemoryManager()
{
	CreateDeviceMemoryBlock(eMemoryBlockUsageGeometry);
	CreateDeviceMemoryBlock(eMemoryBlockUsageImages);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, deviceMemory[0].buffer, &memRequirements);
	deviceDataAlignment[eMemoryBlockUsageGeometry] = memRequirements.alignment;

	SMemoryBlock stagingBlock{};
	CreateBuffer(MEMORY_BLOCK_SIZE, eMemoryBlockUsageStaging,
		stagingBlock.buffer, stagingBlock.memory);
	auto resultA = vkMapMemory(device, stagingBlock.memory, 0, MEMORY_BLOCK_SIZE, 
		0, &stagingBlock.mappedLocation);
	if (resultA != VK_SUCCESS)
		throw std::runtime_error("[ERROR] Failed to map staging block A memory.");
	stagingMemory.push_back(stagingBlock);

	SMemoryBlock stagingBlockB{};
	CreateBuffer(MEMORY_BLOCK_SIZE, eMemoryBlockUsageStaging,
		stagingBlockB.buffer, stagingBlockB.memory);
	auto resultB = vkMapMemory(device, stagingBlockB.memory, 0, MEMORY_BLOCK_SIZE, 
		0, &stagingBlockB.mappedLocation);
	if (resultB != VK_SUCCESS)
		throw std::runtime_error("[ERROR] Failed to map staging block B memory.");
	stagingMemory.push_back(stagingBlockB);
}

void RenderSystem::DestroyDeviceMemoryManager()
{
	for (SMemoryBlock block : deviceMemory)
	{
		vkDestroyBuffer(device, block.buffer, nullptr);
		vkFreeMemory(device, block.memory, nullptr);
	}

	for (SMemoryBlock block : stagingMemory)
	{
		vkUnmapMemory(device, block.memory);
		vkDestroyBuffer(device, block.buffer, nullptr);
		vkFreeMemory(device, block.memory, nullptr);
	}
}


/**** Utility. ****/

uint32 RenderSystem::CreateDeviceMemoryBlock(EMemoryBlockUsage usage)
{
	SMemoryBlock deviceBlock{};
	deviceBlock.usage = usage;
	switch (usage)
	{
		case eMemoryBlockUsageGeometry:
			CreateBuffer(MEMORY_BLOCK_SIZE, usage, deviceBlock.buffer, deviceBlock.memory);
			break;
		case eMemoryBlockUsageUniforms:
			CreateBuffer(MEMORY_BLOCK_SIZE, usage, deviceBlock.buffer, deviceBlock.memory);
			break;
		case eMemoryBlockUsageImages:
			AllocateDeviceMemory(MEMORY_BLOCK_SIZE, memoryBlockUsageLocation[usage], deviceBlock.memory);
			break;
		case eMemoryBlockUsageStaging:
			break;
	}
	deviceMemory.push_back(deviceBlock);
	return static_cast<uint32>(deviceMemory.size() - 1);
}

void RenderSystem::DeviceMalloc(VkDeviceSize size, EMemoryBlockUsage usage, VkDeviceSize& offset, uint32& block)
{
	for (size_t i = 0; i < deviceMemory.size(); i++)
	{
		if (usage != deviceMemory[i].usage)
			continue;

		VkDeviceSize alignment = deviceDataAlignment[usage];

		size_t potentialOffset = deviceMemory[i].cursor % alignment == 0
									 ? deviceMemory[i].cursor
									 : deviceMemory[i].cursor + (alignment - (deviceMemory[i].cursor % alignment));
		size_t available = MEMORY_BLOCK_SIZE - potentialOffset - 1;

		if (available >= size)
		{
			block = static_cast<uint32>(i);
			offset = potentialOffset;
			deviceMemory[i].cursor = potentialOffset + size;
			return;
		}
	}

	block = static_cast<uint32>(deviceMemory.size());
	offset = 0;
	deviceMemory[block].cursor += size;

	CreateDeviceMemoryBlock(usage);
}

void RenderSystem::AllocateDeviceMemory(VkDeviceSize size, EMemoryLocation location, VkDeviceMemory& deviceMemory)
{
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = size;
	allocInfo.memoryTypeIndex = deviceMemoryTypeByLocation[location];

	if (vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to allocate device memory!");
	}
}

void RenderSystem::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &image));

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory));

	vkBindImageMemory(device, image, imageMemory, 0);
}

void RenderSystem::CreateBuffer(
	VkDeviceSize size, EMemoryBlockUsage usage, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = memoryBlockUsageVkFlags[usage];
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create buffer!");
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);
	AllocateDeviceMemory(memoryRequirements.size, memoryBlockUsageLocation[usage], bufferMemory);
	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void RenderSystem::CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}

void RenderSystem::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32 width, uint32 height)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {width, height, 1};

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	EndSingleTimeCommands(commandBuffer);
}

void RenderSystem::CreateTextureMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageMemoryBarrier &barrier)
{
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	}
	else
	{
		std::cout << "[ERROR] Unsupported image layout transition!" << std::endl;
	}
}

void RenderSystem::TransitionImageLayout(VkImage image, VkFormat, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;	  // TODO
	barrier.dstAccessMask = 0;	  // TODO

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("[ERROR] Unsupported layout transition!");
	}

	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	EndSingleTimeCommands(commandBuffer);
}

VkCommandBuffer RenderSystem::BeginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void RenderSystem::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void RenderSystem::GenerateMissingTexture(uint32 width, uint32 tiles, void* textureMemory)
{
	uint32* img = reinterpret_cast<uint32*>(textureMemory);
	uint32 stride = width / tiles;

	for (uint32 y = 0; y < width; y++)
	{
		for (uint32 x = 0; x < width; x++)
		{
			uint32 xTile = x / stride;
			uint32 yTile = y / stride;
			img[x + width * y] = (xTile % 2) ^ (yTile % 2) ? 0xFFFF00FF : 0xFF000000;
		}
	}
}

VkImageView RenderSystem::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
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

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create image view!");
	}

	return imageView;
}

std::vector<char> RenderSystem::LoadSPIRV(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("[ERROR] Failed to open shader file!");
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

VkShaderModule RenderSystem::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo{};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create shader module!");
	}

	return shaderModule;
}

void RenderSystem::ConfigureDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info)
{
	info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
					   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info.pfnUserCallback = DebugCallback;
	info.pUserData = win32Console;
}