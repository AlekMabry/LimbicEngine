#pragma once

#include "EEntity.h"
#include "GLFWIO.h"
#include "ResourceManager.h"
#include "VulkanRenderer.h"

class Game
{
public:
	Game();
	void Run();
	ResourceManager* GetResourceManager() const;

protected:
	std::string name;
	std::unique_ptr<ResourceManager> resources;
	std::unique_ptr<GLFWIO> io;
	std::unique_ptr<VulkanRenderer> renderer;
	std::vector<EEntity> entities;
};