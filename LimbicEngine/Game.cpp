#include "Game.h"

Game::Game()
{
	resources = std::make_unique<ResourceManager>();
	io = std::make_unique<GLFWIO>("Limbic Engine", 1280, 720);
	renderer = std::make_unique<VulkanRenderer>();
}

void Game::Run()
{
}

ResourceManager* Game::GetResourceManager() const
{
	return resources.get();
}