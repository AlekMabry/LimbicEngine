#include "IOSystem_GLFW.h"

#include <bitset>
#include <iostream>

IOSystem_GLFW::IOSystem_GLFW(const char* applicationName)
{
	heldActionFlags = 0;
	glfwInit();

	uint32 extensionCount;
	const char** instanceExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	VkCreateInstance(vkContext.instance, applicationName, instanceExtensions, extensionCount, bDebugEnabled);
	// VkPickPhysicalDevice
	// VkCreateDevice
	// VkPickQueues
}

IOSystem_GLFW::~IOSystem_GLFW()
{
	vkDestroyInstance(vkContext.instance, nullptr);

	for (SWindowGlfw window : windows)
	{
		glfwDestroyWindow(window.window);
	}
	glfwTerminate();
}

HWindow IOSystem_GLFW::SpawnWindow(const char* title, int32 width, int32 height, uint32 drawOptionFlags)
{
	SWindowGlfw window;
	window.drawOptionFlags = drawOptionFlags;
	window.updateFlags = WINDOW_UPDATE_DRAW_FLAG;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window.window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetWindowUserPointer(window.window, static_cast<void*>(this));

	glfwCreateWindowSurface(*pInstance, window.window, NULL, &window.surface);

	HWindow hWindow = static_cast<uint32>(windows.size());
	windows.push_back(window);
	return hWindow;
}

void IOSystem_GLFW::OnTick(float dt)
{
	glfwPollEvents();

	// For now only receive input from the first window
	for (size_t action = 0; action < getKeyByAction.size(); action++)
	{
		int32 state = glfwGetKey(windows[0].window, getGlfwKeyByKey[getKeyByAction[action]]);
		switch (state)
		{
		case GLFW_RELEASE:
			heldActionFlags &= ~(1 << action);	// Clear flag
			break;
		case GLFW_PRESS:
			heldActionFlags |= (1 << action);	// Set flag
			break;
		}
	}
}

void IOSystem_GLFW::BindActions(EInput* inputs, uint32 actionCount)
{
	getKeyByAction.clear();
	for (uint32 i = 0; i < actionCount; i++)
	{
		getKeyByAction.push_back(inputs[i]);
	}
}

void IOSystem_GLFW::SetWindowCamera(HWindow window, mat4& cameraTransform)
{
	windows[window].cameraTransform = cameraTransform;
}

VkSurfaceKHR* IOSystem_GLFW::GetVkSurfaceKHR(HWindow window)
{
	return &windows[window].surface;
}

void IOSystem_GLFW::GetFramebufferSize(HWindow window, int32& width, int32& height)
{
	glfwGetFramebufferSize(windows[window].window, &width, &height);
}

uint32 IOSystem_GLFW::GetWindowUpdateFlags(HWindow window)
{
	return windows[window].updateFlags;
}

uint32 IOSystem_GLFW::GetWindowDrawOptionFlags(HWindow window)
{
	return windows[window].drawOptionFlags;
}

bool IOSystem_GLFW::IsActionHeld(uint64 action)
{
	bool bHeld = heldActionFlags & (1 << action);
	return bHeld;
}

void IOSystem_GLFW::CreateVkContext()
{
	
}