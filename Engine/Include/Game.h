#pragma once

#include <Entity/EStaticWorldGeometry.h>
#include <LimbicTypes.h>
#include <Entity/EEntity.h>
#include <System/IOSystem.h>
#include <System/ResourceSystem.h>
#include <System/WorldSystem.h>
#include <System/RenderSystem.h>
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