#pragma once

#define XR_USE_PLATFORM_WIN32
#define XR_USE_GRAPHICS_API_VULKAN

#include <vulkan/vulkan.h>
#include <Windows.h>
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

inline const char* GetXRErrorString(XrInstance xrInstance, XrResult result)
{
	static char string[XR_MAX_RESULT_STRING_SIZE];
	xrResultToString(xrInstance, result, string);
	return string;
}

#define OPENXR_CHECK(x, y)                                                            \
{                                                                                     \
	XrResult result = (x);                                                            \
	if (!XR_SUCCEEDED(result))                                                        \
	{                                                                                 \
		std::cerr << "ERROR: OPENXR: " << int(result);                                \
		std::cerr << "(" << (xrInstance ? GetXRErrorString(xrInstance, result) : ""); \
		std::cerr << ") " << y << std::endl;                                          \
		throw std::runtime_error("[ERROR] OpenXR error!");                            \
	}                                                                                 \
}


