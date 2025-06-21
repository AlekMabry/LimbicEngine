#include <Game.h>
#include <System/ResourceSystem.h>
#include <System/RenderSystem.h>
#include <Renderer/RWindow.h>
#include <System/WorldSystem.h>
#include <LimbicTypes.h>
#include <Entity/EEntity.h>

#include <chrono>

Game::Game(const std::string& applicationName)
	: dt(0), applicationName(applicationName)
{
	render = std::make_unique<RenderSystem>(this);
	resource = std::make_unique<ResourceSystem>(render.get());
	world = std::make_unique<WorldSystem>(*this);
}

Game::~Game()
{
}

void Game::Run()
{
	LoadMap("");

	auto lastTickTime = std::chrono::high_resolution_clock::now();

	while (true)
	{
		auto currentTickTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTickTime - lastTickTime).count();
		lastTickTime = currentTickTime;

		// todo - Move to window: pIo->OnTick(dt);
		world->OnTick(dt);

		Draw();
	}
}

void Game::LoadMap(const std::string&)
{
	world->LoadFromJSON("C:/Users/alekm/AppData/Local/Outpost731/Map/Test.json");

	for (auto& entity : world->GetEntities())
	{
		entity->OnInit();
	}
}

void Game::Draw()
{
	render->OnDrawStart();
	world->OnDraw();
	for (auto& window : windows)
	{
		window.second->DrawFrame();
	}
}

void Game::CreateWin(const std::string& id, int width, int height)
{
	if (!render->IsInstanceInitialized())
	{
		render->InitInstance(applicationName.c_str());
	}

	windows.emplace(id, std::make_unique<RWindow>(this, applicationName, width, height));

	if (!render->IsSystemInitialized())
	{
		render->InitSystem();
	}

	auto& lWindow = windows.at(id);
	lWindow->Init();
}

RWindow* Game::GetWin(const std::string& name)
{
	if (windows.contains(name))
	{
		return windows.at(name).get();
	}
	return nullptr;
}

std::map<std::string, std::unique_ptr<RWindow>>& Game::GetWins()
{
	return windows;
}

ResourceSystem* Game::GetResourceSystem()
{
	return resource.get();
}

RenderSystem* Game::GetRenderSystem()
{
	return render.get();
}

WorldSystem* Game::GetWorldSystem()
{
	return world.get();
}

std::string Game::GetApplicationName()
{
	return applicationName;
}
