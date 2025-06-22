#pragma once

#include <Renderer/RVRUtil.h>

#include <vector>
#include <string>
#include <iostream>

class Game;

class RVR
{
private:
	void CreateInstance();
	void DestroyInstance();
	void CreateDebugMessenger();
	void DestroyDebugMessenger();
	void GetInstanceProperties();
	void GetSystemID();
	void PollSystemEvents();

	XrInstance xrInstance = {};
	std::vector<const char *> activeAPILayers = {};
	std::vector<const char *> activeInstanceExtensions = {};
	std::vector<std::string> apiLayers = {};
	std::vector<std::string> instanceExtensions = {};

	XrDebugUtilsMessengerEXT debugUtilsMessenger = {};

	XrFormFactor formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	XrSystemId systemID = {};
	XrSystemProperties systemProperties = {XR_TYPE_SYSTEM_PROPERTIES};

	bool bApplicationRunning = true;
	bool bSessionRunning = false;

	Game* pGame;
};