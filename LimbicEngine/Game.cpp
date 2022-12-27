#include "Game.h"

Game::Game()
{
	io = std::make_unique<GLFWIO>("Limbic Engine", 1280, 720);
	uint32 width, height;
	io->GetFramebufferSize(width, height);
	renderer = std::make_unique<VulkanRenderer>("Limbic Engine", width, height, io->GetWindow(), io->GetProcess());
	resources = std::make_unique<ResourceManager>(renderer.get());

	EStaticWorldGeometry* truss1 = SpawnEntity<EStaticWorldGeometry>();
	EStaticWorldGeometry* truss2 = SpawnEntity<EStaticWorldGeometry>();
	static_cast<LStaticMeshComponent*>(truss2->GetRoot())->SetPosition(vec3(0.0f, 0.0f, 3.0f));
}

Game::~Game()
{
	for (EEntity* entity : entities)
	{
		delete entity;
	}
}

void Game::Run()
{
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
				resources->RequestStaticMesh(mesh, node), resources->RequestMaterial(baseColor, normal, properties));
		}
	}

	auto lastTickTime = std::chrono::high_resolution_clock::now();
	float angle = 0.0f;

	while (!io->PollExitEvent())
	{
		auto currentTickTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTickTime - lastTickTime).count();
		lastTickTime = currentTickTime;

		angle += dt * radians(15.0f);
		if (angle > radians(360.0f))
		{
			angle -= radians(360.0f);
		}

		uint32 width, height;
		io->GetFramebufferSize(width, height);

		mat4 modelA = rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
		mat4 modelB = translate(rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f)), vec3(0.0f, 0.0f, 2.0f));
		mat4 view = lookAt(vec3(0.0f, 10.0f, 5.0f), vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 1.0f));
		mat4 proj = perspective(radians(45.0f), (float) width / (float) height, 0.1f, 128.0f);
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

		renderer->FrameDraw(drawInfo.size(), drawInfo.data());
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