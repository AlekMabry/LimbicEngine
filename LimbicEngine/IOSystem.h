#pragma once

#include "LimbicTypes.h"
#include "IOSystem_InputBinds.h"
#include <Windows.h>

#define LINPUT_ACTION_SHOOT		1
#define LINPUT_ACTION_JUMP		2
#define LINPUT_ACTION_AIM		3

typedef uint64 LActionFlags;
typedef uint64 LAxisFlags;

class IOSystem
{
public:
	virtual void OnTick(float dt) = 0;

	/** Bind actions to keys.
		@param[in]	keys		Array of key IDs, where the index of each element corresponds to an action. 
		@param[in]	actionCount	Number of actions in keys array. */
	virtual void BindActions(EKey* keys, uint32 actionCount) = 0;

	virtual HWND GetWindow() = 0;

	virtual HINSTANCE GetProcess() = 0;

	virtual void GetFramebufferSize(uint32& width, uint32& height) = 0;

	virtual bool IsActionHeld(uint64 action) = 0;
};
