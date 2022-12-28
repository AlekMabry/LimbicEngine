#pragma once

#include "IOSystem.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <memory>

class IOSystem_GLFW : public IOSystem
{
public:
	IOSystem_GLFW(const char* applicationName, uint32 width, uint32 height);

	~IOSystem_GLFW();

	HWND GetWindow() override;

	HINSTANCE GetProcess() override;

	void GetFramebufferSize(uint32& width, uint32& height) override;

	bool PollExitEvent() override;

protected:
	GLFWwindow* window;
	uint32 windowWidth;
	uint32 windowHeight;
};
