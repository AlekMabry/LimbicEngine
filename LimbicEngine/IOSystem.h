#pragma once

#include "LimbicTypes.h"
#include "vulkan/vulkan.h"

enum EInput
{
	eInput_LCtrl,
	eInput_LShift,
	eInput_LAlt,
	eInput_Space,
	eInput_Tab,
	eInput_Backtick,
	eInput_0,
	eInput_1,
	eInput_2,
	eInput_3,
	eInput_4,
	eInput_5,
	eInput_6,
	eInput_7,
	eInput_8,
	eInput_9,
	eInput_A,
	eInput_B,
	eInput_C,
	eInput_D,
	eInput_E,
	eInput_F,
	eInput_G,
	eInput_H,
	eInput_I,
	eInput_J,
	eInput_K,
	eInput_L,
	eInput_M,
	eInput_N,
	eInput_O,
	eInput_P,
	eInput_Q,
	eInput_R,
	eInput_S,
	eInput_T,
	eInput_U,
	eInput_V,
	eInput_W,
	eInput_X,
	eInput_Y,
	eInput_Z,
	eInput_LMB,
	eInput_RMB
};

typedef uint32 HWindow;

#define WINDOW_DRAW_OPTION_PBR				1
#define WINDOW_DRAW_OPTION_EDITOR			2
#define WINDOW_DRAW_OPTION_DEBUG			4
#define WINDOW_DRAW_OPTION_ORTHOGRAPHIC		8

#define WINDOW_UPDATE_DRAW_FLAG			1
#define WINDOW_UPDATE_RESIZE_FLAG		2

class IOSystem
{
public:
	virtual HWindow SpawnWindow(const char* title, int32 width, int32 height, uint32 drawOptionFlags) = 0;

	virtual void OnTick(float dt) = 0;

	/** Bind actions to keys.
		@param[in]	inputs		Array of input IDs, where the index of each element corresponds to an action.
		@param[in]	actionCount	Number of actions in keys array. */
	virtual void BindActions(EInput* inputs, uint32 actionCount) = 0;

	virtual void SetWindowCamera(HWindow window, mat4& cameraTransform) = 0;

	inline void SetVkInstance(VkInstance* instance)
	{
		pInstance = instance;
	}


	virtual VkSurfaceKHR* GetVkSurfaceKHR(HWindow window) = 0;

	virtual void GetFramebufferSize(HWindow window, int32& width, int32& height) = 0;

	virtual uint32 GetWindowUpdateFlags(HWindow window) = 0;

	virtual uint32 GetWindowDrawOptionFlags(HWindow window) = 0;

	virtual bool IsActionHeld(uint64 action) = 0;

protected:
	VkInstance* pInstance;
};