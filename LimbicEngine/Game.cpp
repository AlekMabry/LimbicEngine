#include "Game.h"

Game::Game()
{
	resources = std::make_unique<ResourceManager>();
	io = std::make_unique<GLFWIO>("Limbic Engine", 1280, 720);
	uint32 width, height;
	io->GetFramebufferSize(width, height);
	renderer = std::make_unique<VulkanRenderer>("Limbic Engine", width, height, io->GetWindow(), io->GetProcess());
	io->SetRenderer(renderer.get());
}

void Game::Run()
{
	io->Run();
}

ResourceManager* Game::GetResourceManager() const
{
	return resources.get();
}