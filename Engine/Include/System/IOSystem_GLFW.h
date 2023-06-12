#pragma once

#include <System/IOSystem.h>
#include <Utility/VkUtil.h>

#include <GLFW/glfw3.h>

#include <map>
#include <memory>
#include <string>

#ifdef _DEBUG
const bool bDebugEnabled = true;
#else
const bool bDebugEnabled = false;
#endif

struct SWindowGlfw
{
	// ---- Glfw ----
	std::string title;
	GLFWwindow* window;

	// ---- Vulkan ----
	SVkWindow vkWindow;

	// ---- Window state. ----
	uint32 drawOptionFlags;
	uint32 updateFlags;
	mat4 cameraTransform;
};

class IOSystem_GLFW : public IOSystem
{
public:
	IOSystem_GLFW(const char* applicationName);

	~IOSystem_GLFW();

	HWindow SpawnWindow(const char* title, int32 width, int32 height, uint32 drawOptionFlags) override;

	void OnTick(float dt) override;

	void BindActions(EInput* inputs, uint32 actionCount) override;

	void SetWindowCamera(HWindow window, mat4& cameraTransform) override;

	VkSurfaceKHR* GetVkSurfaceKHR(HWindow window) override;

	void GetFramebufferSize(HWindow window, int32& width, int32& height) override;

	uint32 GetWindowUpdateFlags(HWindow window) override;

	uint32 GetWindowDrawOptionFlags(HWindow window) override;

	bool IsActionHeld(uint64 action) override;

protected:
	void CreateVkContext();

	// ---- Input ----
	uint64 heldActionFlags;				/** Currently held action flags. */
	std::vector<uint32> getKeyByAction;	/** Get EKey by action index. */

	// ---- Vulkan ----
	SVkContext vkContext;

	// ---- Window management. ----
	std::vector<SWindowGlfw> windows;
};

/** Maps GLFW keys to EInput. */
const uint32 getGlfwKeyByInput[] = {
	GLFW_KEY_LEFT_CONTROL,
	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_LEFT_ALT,
	GLFW_KEY_SPACE,
	GLFW_KEY_TAB,
	GLFW_KEY_GRAVE_ACCENT,
	GLFW_KEY_0,
	GLFW_KEY_1,
	GLFW_KEY_2,
	GLFW_KEY_3,
	GLFW_KEY_4,
	GLFW_KEY_5,
	GLFW_KEY_6,
	GLFW_KEY_7,
	GLFW_KEY_8,
	GLFW_KEY_9,
	GLFW_KEY_A,
	GLFW_KEY_B,
	GLFW_KEY_C,
	GLFW_KEY_D,
	GLFW_KEY_E,
	GLFW_KEY_F,
	GLFW_KEY_G,
	GLFW_KEY_H,
	GLFW_KEY_I,
	GLFW_KEY_J,
	GLFW_KEY_K,
	GLFW_KEY_L,
	GLFW_KEY_M,
	GLFW_KEY_N,
	GLFW_KEY_O,
	GLFW_KEY_P,
	GLFW_KEY_Q,
	GLFW_KEY_R,
	GLFW_KEY_S,
	GLFW_KEY_T,
	GLFW_KEY_U,
	GLFW_KEY_V,
	GLFW_KEY_W,
	GLFW_KEY_X,
	GLFW_KEY_Y,
	GLFW_KEY_Z
};