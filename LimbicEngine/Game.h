#pragma once

#include "LimbicTypes.h"
#include "EEntity.h"
#include "GLFWIO.h"
#include "ResourceManager.h"
#include "VulkanRenderer.h"
#include "World.h"
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
//#define GLM_FORCE_LEFT_HANDED
#define GLM_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct SGameManagers
{
	ResourceManager* resources;
	World* world;
	Game* game;
};

class Game
{
public:
	Game();

	void Run();

	void GetGameManagers(SGameManagers& managers);

protected:
	float dt;
	std::unique_ptr<ResourceManager> resources;
	std::unique_ptr<GLFWIO> io;
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<World> world;
	std::vector<std::unique_ptr<EEntity>> entities;
};