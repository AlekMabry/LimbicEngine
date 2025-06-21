#include "Game.h"
#include "System/RenderSystem.h"
#include "Renderer/RWindow_GLFW.h"
#include "System/IOSystem_GLFW.h"

int main()
{
	Game game("Outpost 731");

	// Create Vulkan instance
	auto pRender = game.GetRenderSystem();
	pRender->InitInstance("Outpost 731");

	// Create window
	IOSystem_GLFW io(game.GetApplicationName().c_str(), 1280, 720);
	game.SetIO(io);
	RWindow_GLFW mainVkWindow(game.GetRenderSystem(), 1280, 720, io.GetWindow(), io.GetProcess());
	mainVkWindow.CreateSurface();
	pRender->AddWindow("main", mainVkWindow);

	// Initialize Vulkan (configure device, graphics/presentation queue)
	pRender->InitGLFW();

	// Initialize RenderSystem (asset management)
	pRender->Init();

	// Initialize RWindow (swapchain, frame/depth buffers, command queues)
	mainVkWindow.Init();

	// Load world, begin game loop
	game.Run();

	return EXIT_SUCCESS;
}