#pragma once

#include <string>
#include <memory>

class ResourceSystem;
class RenderSystem;
class WorldSystem;
class IOSystem;

class Game
{
public:
	Game(const std::string& applicationName);
	~Game();

	void OnInit();

	void SetIO(IOSystem& io);

	void Run();

	ResourceSystem* GetResourceSystem() const;

	RenderSystem* GetRenderSystem();

	WorldSystem* GetWorldSystem() const;

	IOSystem* GetIOSystem() const;

	std::string GetApplicationName() const;

protected:
	float dt;
	std::unique_ptr<ResourceSystem> resource;
	std::unique_ptr<RenderSystem> render;
	std::unique_ptr<WorldSystem> world;
	IOSystem* pIo;
	std::string applicationName;
};