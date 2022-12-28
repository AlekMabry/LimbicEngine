#include "Game.h"

Game::Game(std::string applicationName)
{
	this->applicationName = applicationName;
}

Game::~Game()
{
	for (EEntity* entity : entities)
	{
		delete entity;
	}
}

void Game::SetIO(IOSystem* ioSystem)
{
	this->ioSystem = ioSystem;
}

void Game::Run()
{
	renderSystem = std::make_unique<RenderSystem>();
	resourceSystem = std::make_unique<ResourceSystem>(renderSystem.get());
	uint32 w, h;
	ioSystem->GetFramebufferSize(w, h);
	renderSystem->Init(applicationName.c_str(), w, h, ioSystem->GetWindow(), ioSystem->GetProcess());

	EStaticWorldGeometry* truss1 = SpawnEntity<EStaticWorldGeometry>();
	EStaticWorldGeometry* truss2 = SpawnEntity<EStaticWorldGeometry>();
	static_cast<LStaticMeshComponent*>(truss2->GetRoot())->SetPosition(vec3(0.0f, 0.0f, 3.0f));

	for (EEntity* entity : entities)
	{
		LStaticMeshComponent* root = dynamic_cast<LStaticMeshComponent*>(entity->GetRoot());
		if (root != nullptr)
		{
			std::string mesh;
			std::string node;
			std::string baseColor;
			std::string normal;
			std::string properties;
			root->GetResourceInfo(mesh, node, baseColor, normal, properties);
			root->SetResources(
				resourceSystem->RequestStaticMesh(mesh, node), resourceSystem->RequestMaterial(baseColor, normal, properties));
		}
	}

	auto lastTickTime = std::chrono::high_resolution_clock::now();
	float angle = 0.0f;

	while (!ioSystem->PollExitEvent())
	{
		auto currentTickTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTickTime - lastTickTime).count();
		lastTickTime = currentTickTime;

		angle += dt * glm::radians(15.0f);
		if (angle > glm::radians(360.0f))
		{
			angle -= glm::radians(360.0f);
		}

		uint32 width, height;
		ioSystem->GetFramebufferSize(width, height);

		mat4 modelA = rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
		mat4 modelB = translate(rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f)), vec3(0.0f, 0.0f, 2.0f));
		mat4 view = lookAt(vec3(0.0f, 10.0f, 5.0f), vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 1.0f));
		mat4 proj = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 128.0f);
		proj[1][1] *= -1;

		std::vector<SDrawStaticPBR> drawInfo;

		for (EEntity* entity : entities)
		{
			LStaticMeshComponent* root = dynamic_cast<LStaticMeshComponent*>(entity->GetRoot());
			if (root != nullptr)
			{
				SDrawStaticPBR drawEntity;
				root->GetDrawInfo(drawEntity.mesh, drawEntity.material);
				drawEntity.transform.camera = proj * view;
				drawEntity.transform.model = root->GetLocalTransform();
				drawInfo.push_back(drawEntity);
			}
		}

		renderSystem->FrameDraw(drawInfo.size(), drawInfo.data());
	}
}

template <typename TEntityClass>
TEntityClass* Game::SpawnEntity()
{
	TEntityClass* entity = new TEntityClass();
	entities.push_back(static_cast<EEntity*>(entity));
	return entity;
}

void Game::CleanPendingKills()
{
	size_t cursor = 0;
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (!entities[i]->IsPendingKill())
		{
			entities[cursor] = entities[i];
			cursor = i + 1;
		}
		else
		{
			delete entities[i];
		}
	}
	entities.resize(cursor);
}