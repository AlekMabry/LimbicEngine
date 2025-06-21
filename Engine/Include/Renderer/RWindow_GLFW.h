#pragma once

#include "LimbicTypes.h"
#include "Renderer/RWindow.h"

#include <Windows.h>
#include <vector>
#include <vulkan/vulkan.hpp>

class RenderSystem;

class RWindow_GLFW : public RWindow
{
public:
	RWindow_GLFW(RenderSystem *pRenderSystem, uint32 width, uint32 height, HWND window, HINSTANCE process);
	~RWindow_GLFW();
};
