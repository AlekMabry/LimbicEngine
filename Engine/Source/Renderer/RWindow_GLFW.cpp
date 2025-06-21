#include "Renderer/RWindow_GLFW.h"
#include "Renderer/RView.h"
#include "System/RenderSystem.h"

#include <vulkan/vulkan_win32.h>
#include <stdexcept>

RWindow_GLFW::RWindow_GLFW(RenderSystem* pRenderSystem, uint32 width, uint32 height, HWND window, HINSTANCE process)
	: RWindow(pRenderSystem, width, height, window, process)
{
}

RWindow_GLFW::~RWindow_GLFW()
{
}