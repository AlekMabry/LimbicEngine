#include "Game.h"

Game::Game(std::string applicationName)
{
	this->applicationName = applicationName;
}

void Game::SetIO(IOSystem* ioSystem)
{
	this->ioSystem = ioSystem;
}

void Game::OnInit()
{
	worldSystem = std::make_unique<WorldSystem>();
	renderSystem = std::make_unique<RenderSystem>();
	resourceSystem = std::make_unique<ResourceSystem>(renderSystem.get());

	uint32 w, h;
	ioSystem->GetFramebufferSize(w, h);
	renderSystem->Init(applicationName.c_str(), w, h, ioSystem->GetWindow(), ioSystem->GetProcess());

	worldSystem->LoadFromJSON("C:/Users/alekm/Desktop/Outpost731/Map/Test.json");

	EEntity** entities;
	uint32 entityCount;
	worldSystem->GetEntities(entities, entityCount);
	for (uint32 i = 0; i < entityCount; i++)
	{
		entities[i]->OnInit(resourceSystem.get());
	}
}

void Game::Run()
{
	OnInit();

	auto lastTickTime = std::chrono::high_resolution_clock::now();

	while (!ioSystem->PollExitEvent())
	{
		auto currentTickTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTickTime - lastTickTime).count();
		lastTickTime = currentTickTime;

		EEntity** entities;
		uint32 entityCount = 0;
		worldSystem->GetEntities(entities, entityCount);
		for (uint32 i = 0; i < entityCount; i++)
		{
			entities[i]->OnTick(dt);
		}

		renderSystem->OnDrawStart();

		for (uint32 i = 0; i < entityCount; i++)
		{
			entities[i]->OnDraw(renderSystem.get());
		}

		renderSystem->OnDrawEnd();
	}
}