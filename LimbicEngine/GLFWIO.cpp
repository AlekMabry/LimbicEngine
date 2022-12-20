#include "GLFWIO.h"

GLFWIO::GLFWIO(const char* applicationName, uint32 width, uint32 height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr);
}

HWND GLFWIO::GetWindow()
{
	return glfwGetWin32Window(window);
}

HINSTANCE GLFWIO::GetProcess()
{
	return GetModuleHandle(nullptr);
}

void GLFWIO::GetFramebufferSize(uint32& width, uint32& height)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	width = w;
	height = h;
}

GLFWIO::~GLFWIO()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool GLFWIO::PollExitEvent()
{
	glfwPollEvents();
	return glfwWindowShouldClose(window);
}