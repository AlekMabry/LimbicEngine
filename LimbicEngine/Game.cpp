#include "Game.h"

Game::Game()
{
	resources = std::make_unique<ResourceManager>();
	io = std::make_unique<GLFWIO>("Limbic Engine", 1280, 720);
	uint32 width, height;
	io->GetFramebufferSize(width, height);
	renderer = std::make_unique<VulkanRenderer>("Limbic Engine", width, height, io->GetWindow(), io->GetProcess());
}

void Game::Run()
{
	std::string fbx = "C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx";
	std::string object = "Truss";
	SMesh* trussData = resources->LoadMesh(fbx, object);

	uint32 trussMesh;
	SStaticVertex* verticesPtr;
	uint32* indicesPtr;
	renderer->CreateStaticMesh(trussData->vertices.size(), trussData->indices.size(), trussMesh, verticesPtr, indicesPtr);
	memcpy(verticesPtr, trussData->vertices.data(), sizeof(SStaticVertex) * trussData->vertices.size());
	memcpy(indicesPtr, trussData->indices.data(), sizeof(uint32) * trussData->indices.size());

	uint32 trussTexture;
	void* texPtr;
	renderer->CreateTexture(1024, 1024, eTextureFormatDXT1, trussTexture, texPtr);
	std::string ktx = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_BaseColor.KTX2";
	resources->LoadTextureKTX2(ktx, texPtr);

	renderer->SubmitAssets();

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

		mat4 model = rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
		mat4 view = lookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
		mat4 proj = perspective(radians(45.0f), (float) width / (float) height, 0.1f, 10.0f);
		proj[1][1] *= -1;

		mat4 mvp = proj * view * model;

		std::vector<SDrawPBRMesh> staticMeshes;
		staticMeshes.push_back({0, 0, 0, 0, 0, mvp});
		renderer->FrameSetStaticPBRMeshes(static_cast<uint32>(staticMeshes.size()), staticMeshes.data());
		renderer->FrameDraw();
	}
}

ResourceManager* Game::GetResourceManager() const
{
	return resources.get();
}