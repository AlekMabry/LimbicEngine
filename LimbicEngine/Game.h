#pragma once

#include "EStaticWorldGeometry.h"
#include "LimbicTypes.h"
#include "EEntity.h"
#include "IOSystem.h"
#include "ResourceSystem.h"
#include "WorldSystem.h"
#include "RenderSystem.h"
#include <chrono>

class Game
{
public:
	Game(std::string applicationName, IOSystem* pIO);

	void OnInit();

	void Run();

protected:
	void CreateVkInstance();

	bool CheckVkInstanceExtensionSupport(const std::vector<const char*>& extensions);
	
	VkInstance vkInstance;
	float dt;
	std::unique_ptr<ResourceSystem> resourceSystem;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<WorldSystem> worldSystem;
	IOSystem* pIO;
	std::string applicationName;
};