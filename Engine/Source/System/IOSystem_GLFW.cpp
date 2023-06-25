#include <System/IOSystem_GLFW.h>
#include <System/IOSystem_GLFWBinds.h>

#include <bitset>
#include <iostream>

IOSystem_GLFW::IOSystem_GLFW(const char* applicationName, uint32 width, uint32 height)
{
	heldActionFlags = 0;
	this->windowWidth = width;
	this->windowHeight = height;
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr);
	glfwSetWindowUserPointer(window, static_cast<void*>(this));
}

void IOSystem_GLFW::OnTick(float)
{
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
		abort();

	for (size_t action = 0; action < getKeyByAction.size(); action++)
	{
		int32 state = glfwGetKey(window, getGlfwKeyByKey[getKeyByAction[action]]);
		switch (state)
		{
		case GLFW_RELEASE:
			heldActionFlags &= ~(1i64 << action);	// Clear flag
			break;
		case GLFW_PRESS:
			heldActionFlags |= static_cast<uint64>(static_cast<uint64>(1) << action);	// Set flag
			break;
		}
	}
}

void IOSystem_GLFW::BindActions(EKey* keys, uint32 actionCount)
{
	getKeyByAction.clear();
	for (uint32 i = 0; i < actionCount; i++)
	{
		getKeyByAction.push_back(keys[i]);
	}
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

bool IOSystem_GLFW::IsActionHeld(uint64 action)
{
	bool bHeld = heldActionFlags & (static_cast<unsigned __int64>(1) << action);
	return bHeld;
}