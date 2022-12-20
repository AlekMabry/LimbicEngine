#pragma once

#include "LimbicTypes.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <memory>

class GLFWIO
{
public:
	GLFWIO(const char* applicationName, uint32 width, uint32 height);

	HWND GetWindow();
	HINSTANCE GetProcess();
	void GetFramebufferSize(uint32& width, uint32& height);
	bool PollExitEvent();

	~GLFWIO();

protected:
	GLFWwindow* window;
	uint32 windowWidth;
	uint32 windowHeight;
};
