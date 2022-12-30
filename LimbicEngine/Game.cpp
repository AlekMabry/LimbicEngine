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

	/*
	EStaticWorldGeometry* truss[2];
	for (uint32 i = 0; i < 2; i++)
	{
		uint32 id;
		truss[i] = worldSystem->SpawnEntity<EStaticWorldGeometry>(std::string("Truss"), id);
		truss[i]->staticMeshFilename = std::string("C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx");
		truss[i]->staticMeshNode = std::string("Truss");
		truss[i]->baseColorFilename = std::string("C:/Users/alekm/Desktop/Outpost731/Textures/Truss_BaseColor.KTX2");
		truss[i]->normalFilename = std::string("C:/Users/alekm/Desktop/Outpost731/Textures/Truss_Normal.KTX2");
		truss[i]->propertiesFilename = std::string("C:/Users/alekm/Desktop/Outpost731/Textures/Truss_Properties.KTX2");
		truss[i]->position = vec3(0.0f, 0.0f, 0.0f);
	}
	truss[1]->position = vec3(0.0f, 0.0f, 2.0f);
	*/

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

		uint32 width, height;
		ioSystem->GetFramebufferSize(width, height);

		mat4 view = lookAt(vec3(0.0f, 10.0f, 5.0f), vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 1.0f));
		mat4 proj = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 128.0f);
		proj[1][1] *= -1;

		renderSystem->DrawSetCamera(proj * view);

		for (uint32 i = 0; i < entityCount; i++)
		{
			entities[i]->OnDraw(renderSystem.get());
		}

		renderSystem->OnDrawEnd();
	}
}