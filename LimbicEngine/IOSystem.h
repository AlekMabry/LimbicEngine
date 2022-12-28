#pragma once

#include "LimbicTypes.h"
#include <Windows.h>

class IOSystem
{
public:
	virtual HWND GetWindow() = 0;
	virtual HINSTANCE GetProcess() = 0;
	virtual void GetFramebufferSize(uint32& width, uint32& height) = 0;
	virtual bool PollExitEvent() = 0;
};
