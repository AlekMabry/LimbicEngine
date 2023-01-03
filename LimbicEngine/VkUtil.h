#pragma once

#include "LimbicTypes.h"
#include "vulkan/vulkan.h"

#define VK_CHECK(f)                                                 \
	{                                                               \
		VkResult res = (f);                                         \
		if (res != VK_SUCCESS)                                      \
		{                                                           \
			std::cout << "[ERROR] Line: " << __LINE__ << std::endl; \
			assert(res == VK_SUCCESS);                              \
		}                                                           \
	}

VkInstance VkCreateInstance(const char** extensions);

bool VkCheckInstanceExtensionSupport(const char** extensions, uint64 extensionCount);

bool VkCheckValidationLayerSupport(const char** extensions);