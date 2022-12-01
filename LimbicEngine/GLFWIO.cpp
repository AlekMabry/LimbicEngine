#include "GLFWIO.h"

GLFWIO::GLFWIO(const char* applicationName, uint32 width, uint32 height)
{
	this->windowWidth = width;
	this->windowHeight = height;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr);
}

void GLFWIO::Run(std::is_function<void()> run)
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		run();
	}
}

GLFWIO::~GLFWIO()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}