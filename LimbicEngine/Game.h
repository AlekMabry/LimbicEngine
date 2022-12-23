#pragma once

#include "LimbicTypes.h"
#include "EEntity.h"
#include "GLFWIO.h"
#include "ResourceManager.h"
#include "VulkanRenderer.h"
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
//#define GLM_FORCE_LEFT_HANDED
#define GLM_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#pragma pack(1)
struct SBC1Block
{
	unsigned short color[2];
	char indices[4];
};

using namespace glm;

class Game
{
public:
	Game();
	void Run();
	ResourceManager* GetResourceManager() const;

protected:
	float dt;
	std::unique_ptr<ResourceManager> resources;
	std::unique_ptr<GLFWIO> io;
	std::unique_ptr<VulkanRenderer> renderer;
	std::vector<EEntity> entities;
};