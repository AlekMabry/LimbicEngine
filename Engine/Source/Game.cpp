#include <Game.h>
#include <System/ResourceSystem.h>
#include <System/RenderSystem.h>
#include <System/WorldSystem.h>
#include <System/IOSystem.h>
#include <LimbicTypes.h>
#include <Entity/EEntity.h>

#include <chrono>

Game::Game(const std::string& applicationName)
	: dt(0), pIo(nullptr) , applicationName(applicationName)
{
}

Game::~Game()
{
}

void Game::SetIO(IOSystem& io)
{
	pIo = &io;
}

void Game::OnInit()
{ 
	render = std::make_unique<RenderSystem>();
	resource = std::make_unique<ResourceSystem>(render.get());
	
	uint32 w, h;
	pIo->GetFramebufferSize(w, h);
	render->Init(applicationName.c_str(), w, h, pIo->GetWindow(), pIo->GetProcess());

	world = std::make_unique<WorldSystem>(*this);
	// I tried to get a windows env path and it gave me wide chars and instability so this is hardcoded for the time being
	world->LoadFromJSON("C:/Users/alekm/AppData/Local/Outpost731/Map/Test.json");

	EEntity** entities;
	uint32 entityCount;
	world->GetEntities(entities, entityCount);
	for (uint32 i = 0; i < entityCount; i++)
	{
		entities[i]->OnInit();
	}
}

void Game::Run()
{
	OnInit();

	auto lastTickTime = std::chrono::high_resolution_clock::now();

	while (true)
	{
		auto currentTickTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTickTime - lastTickTime).count();
		lastTickTime = currentTickTime;

		pIo->OnTick(dt);
		world->OnTick(dt);

		render->OnDrawStart();
		world->OnDraw();
		render->OnDrawEnd();
	}
}

ResourceSystem* Game::GetResourceSystem() const
{
	return resource.get();
}

RenderSystem* Game::GetRenderSystem() const
{
	return render.get();
}

WorldSystem* Game::GetWorldSystem() const
{
	return world.get();
}

IOSystem* Game::GetIOSystem() const
{
	return pIo;
}

std::string Game::GetApplicationName() const
{
	return applicationName;
}
