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

using namespace glm;

const SStaticVertex pyramid[9] = {
	{{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.000096, 0.381989f}, {0.0f, 0.0f}},
	{{0.0f, 1.0f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.35685f, 0.745309f}, {0.0f, 0.0f}},
	{{0.866025f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.441068f, 0.127394f}, {0.0f, 0.0f}},

	{{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.797821f, 0.999903f}, {0.0f, 0.0f}},
	{{-0.866025f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.934088f, 0.509286f}, {0.0f, 0.0f}},
	{{0.0f, 1.0f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.35685f, 0.745309f}, {0.0f, 0.0f}},

	{{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.934088, 0.000097}, {0.0f, 0.0f}},
	{{0.866025f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.441f, 0.127f}, {0.0f, 0.0f}},
	{{0.866025f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.934088f, 0.509286f},
		{0.0f, 0.0f}}};

const uint32 pyramidIndices[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

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