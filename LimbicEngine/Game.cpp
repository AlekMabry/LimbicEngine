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
	//std::string fbx = "C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx";
	//std::string object = "Truss";
	//resources->LoadMesh(fbx, object);
	//uint32 trussMesh;
	
	//uint32 pyramidMesh;
	//SStaticVertex* verticesPtr;
	//uint32* indicesPtr;
	//renderer->CreateStaticMesh(9, 9, pyramidMesh, verticesPtr, indicesPtr);
	//memcpy(verticesPtr, pyramid, sizeof(SStaticVertex) * 9);
	//memcpy(indicesPtr, pyramidIndices, sizeof(uint32) * 9);
	//renderer->SubmitStaticMeshes();

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

		//std::vector<SDrawPBRMesh> staticMeshes;
		//staticMeshes.push_back({0, 0, 0, 0, 0, mvp});
		//renderer->FrameSetStaticPBRMeshes(static_cast<uint32>(staticMeshes.size()), staticMeshes.data());
		//renderer->FrameDraw();
	}
}

ResourceManager* Game::GetResourceManager() const
{
	return resources.get();
}