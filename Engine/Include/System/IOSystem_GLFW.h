#pragma once

#include <System/IOSystem.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <map>
#include <memory>

class IOSystem_GLFW : public IOSystem
{
public:
	IOSystem_GLFW(const char* applicationName, uint32 width, uint32 height);

	~IOSystem_GLFW();

	void OnTick(float dt) override;

	void BindActions(EKey* keys, uint32 actionCount) override;

	HWND GetWindow() override;

	HINSTANCE GetProcess() override;

	void GetFramebufferSize(uint32& width, uint32& height) override;

	bool IsActionHeld(uint64 action) override;

protected:
	GLFWwindow* window;
	uint32 windowWidth;
	uint32 windowHeight;

	uint64 heldActionFlags;				/** Currently held action flags. */
	std::vector<uint32> getKeyByAction;	/** Get EKey by action index. */
};
