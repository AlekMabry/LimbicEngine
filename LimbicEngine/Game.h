#pragma once

#include "EStaticWorldGeometry.h"
#include "LimbicTypes.h"
#include "EEntity.h"
#include "IOSystem.h"
#include "ResourceSystem.h"
#include "WorldSystem.h"
#include "RenderSystem.h"
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
//#define GLM_FORCE_LEFT_HANDED
#define GLM_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

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