#include "Game.h"

Game::Game()
{
	resources = std::make_unique<ResourceManager>();
	io = std::make_unique<GLFWIO>("Limbic Engine", 1280, 720);
	uint32 width, height;
	io->GetFramebufferSize(width, height);
	renderer = std::make_unique<VulkanRenderer>("Limbic Engine", width, height, io->GetWindow(), io->GetProcess());
	world = std::make_unique<World>();
}

void Game::Run()
{
	world->

	std::string fbx = "C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx";
	std::string object = "Truss";
	SMesh* trussData = resources->LoadMesh(fbx, object);

	RStaticMesh trussMesh;
	SStaticVertex* verticesPtr;
	uint32* indicesPtr;
	renderer->CreateStaticMesh(trussData->vertices.size(), trussData->indices.size(), trussMesh, verticesPtr, indicesPtr);
	memcpy(verticesPtr, trussData->vertices.data(), sizeof(SStaticVertex) * trussData->vertices.size());
	memcpy(indicesPtr, trussData->indices.data(), sizeof(uint32) * trussData->indices.size());

	RTexture trussBaseColorTexture;
	RTexture trussNormalTexture;
	RTexture trussPropertiesTexture;

	std::string baseColorPath = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_BaseColor.KTX2";
	std::string trussNormalPath = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_Normal.KTX2";
	std::string trussPropertiesPath = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_Properties.KTX2";

	void* texPtr;

	renderer->CreateTexture(1024, 1024, eTextureFormatDXT1, trussBaseColorTexture, texPtr);
	resources->LoadTextureKTX2(baseColorPath, texPtr);
	renderer->CreateTexture(1024, 1024, eTextureFormatDXT1, trussNormalTexture, texPtr);
	resources->LoadTextureKTX2(trussNormalPath, texPtr);
	renderer->CreateTexture(1024, 1024, eTextureFormatDXT1, trussPropertiesTexture, texPtr);
	resources->LoadTextureKTX2(trussPropertiesPath, texPtr);

	renderer->SubmitAssets();

	RMaterial trussMaterial;
	renderer->CreateMaterial(trussBaseColorTexture, trussNormalTexture, trussPropertiesTexture, trussMaterial);

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

		SDrawStaticPBR draw[2];
		draw[0].mesh = trussMesh;
		draw[0].material = trussMaterial;
		draw[0].transform.camera = proj * view;
		draw[0].transform.model = modelA;

		draw[1].mesh = trussMesh;
		draw[1].material = trussMaterial;
		draw[1].transform.camera = proj * view;
		draw[1].transform.model = modelB;

		renderer->FrameDraw(2, draw);
	}
}

void Game::GetGameManagers(SGameManagers& managers)
{
	managers.resources = resources.get();
	managers.world = world.get();
	managers.game = this;
}