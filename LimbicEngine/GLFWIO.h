#pragma once

#include "LimbicTypes.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <memory>
#include "VulkanRenderer.h"

class GLFWIO
{
public:
	GLFWIO(const char* applicationName, uint32 width, uint32 height);

	void Run();
	void SetRenderer(VulkanRenderer* renderer);
	HWND GetWindow();
	HINSTANCE GetProcess();
	void GetFramebufferSize(uint32& width, uint32& height);
	static void ResizeCallback(GLFWwindow* window, int width, int height);

	~GLFWIO();

protected:
	VulkanRenderer* renderer;
	GLFWwindow* window;
	uint32 windowWidth;
	uint32 windowHeight;
};
