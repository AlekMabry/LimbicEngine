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
	Game(std::string applicationName);

	void OnInit();

	void SetIO(IOSystem* ioSystem);

	void Run();

protected:
	float dt;
	std::unique_ptr<ResourceSystem> resourceSystem;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<WorldSystem> worldSystem;
	IOSystem* ioSystem;
	std::string applicationName;
};