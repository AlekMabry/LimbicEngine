#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
	uint32 extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::cout << "Vk extensions: " << extensionCount << std::endl;
}