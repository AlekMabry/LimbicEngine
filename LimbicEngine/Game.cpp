#include "Game.h"

Game::Game()
{
	resources = std::make_unique<ResourceManager>();
}

void Game::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

ResourceManager* Game::GetResourceManager() const
{
	return resources.get();
}