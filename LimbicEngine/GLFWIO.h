#pragma once

#include "LimbicTypes.h"

#include <GLFW/glfw3.h>

#include <memory>

class GLFWIO
{
public:
	GLFWIO(const char* applicationName, uint32 width, uint32 height);
	void Run(std::is_function<void()> run);
	~GLFWIO();

protected:
	GLFWwindow *window;
	uint32 windowWidth;
	uint32 windowHeight;
};
