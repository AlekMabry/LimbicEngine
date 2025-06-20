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
	render = std::make_unique<RenderSystem>();
	resource = std::make_unique<ResourceSystem>(render.get());
	world = std::make_unique<WorldSystem>(*this);
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
	// I tried to get a windows env path and it gave me wide chars and instability so this is hardcoded for the time being
	world->LoadFromJSON("C:/Users/alekm/AppData/Local/Outpost731/Map/Test.json");

	for (auto& entity : world->GetEntities())
	{
		entity->OnInit();
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

		Draw();
	}
}

void Game::Draw()
{
	render->OnDrawStart();
	world->OnDraw();
	render->OnDrawEnd();
}

ResourceSystem* Game::GetResourceSystem() const
{
	return resource.get();
}

RenderSystem* Game::GetRenderSystem()
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
