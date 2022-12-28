#include "IOSystem_GLFW.h"

IOSystem_GLFW::IOSystem_GLFW(const char* applicationName, uint32 width, uint32 height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr);
}

HWND IOSystem_GLFW::GetWindow()
{
	return glfwGetWin32Window(window);
}

HINSTANCE IOSystem_GLFW::GetProcess()
{
	return GetModuleHandle(nullptr);
}

void IOSystem_GLFW::GetFramebufferSize(uint32& width, uint32& height)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	width = w;
	height = h;
}

IOSystem_GLFW::~IOSystem_GLFW()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool IOSystem_GLFW::PollExitEvent()
{
	glfwPollEvents();
	return glfwWindowShouldClose(window);
}