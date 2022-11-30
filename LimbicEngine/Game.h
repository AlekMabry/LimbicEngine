#pragma once

#include "EEntity.h"
#include "ResourceManager.h"

#include <glad/glad.h>

class Game
{
public:
	Game();
	void Render();
	ResourceManager* GetResourceManager() const;

protected:
	std::string name;
	std::unique_ptr<ResourceManager> resources;
	std::vector<EEntity> entities;
};