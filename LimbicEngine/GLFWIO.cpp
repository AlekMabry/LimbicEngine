#include "GLFWIO.h"

GLFWIO::GLFWIO(const char* applicationName, uint32 width, uint32 height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->renderer = nullptr;
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);
}

void GLFWIO::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		renderer->DrawFrame();
	}
}

void GLFWIO::SetRenderer(VulkanRenderer* renderer)
{
	this->renderer = renderer;
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

void GLFWIO::ResizeCallback(GLFWwindow* window, int width, int height)
{
	auto io = reinterpret_cast<GLFWIO*>(glfwGetWindowUserPointer(window));
	if (io->renderer != nullptr)
		io->renderer->FramebufferResized();
}