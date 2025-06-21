#pragma once

#include <string>
#include <memory>
#include <map>

class RWindow;
class ResourceSystem;
class RenderSystem;
class WorldSystem;

class Game
{
public:
	Game(const std::string& applicationName);
	~Game();

	void Run();

	void LoadMap(const std::string& fname);

	void Draw();

	void CreateWin(const std::string& id, int width, int height);

	RWindow* GetWin(const std::string& name);

	std::map<std::string, std::unique_ptr<RWindow>>& GetWins();

	ResourceSystem* GetResourceSystem();

	RenderSystem* GetRenderSystem();

	WorldSystem* GetWorldSystem();

	std::string GetApplicationName();

protected:
	float dt;
	std::unique_ptr<ResourceSystem> resource;
	std::unique_ptr<RenderSystem> render;
	std::unique_ptr<WorldSystem> world;
	std::string applicationName;
	std::map<std::string, std::unique_ptr<RWindow>> windows;
};