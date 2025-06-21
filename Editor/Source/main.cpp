#include <QApplication>
#include <QVulkanInstance>
#include "EditorWindow.h"
#include "Game.h"
#include "System/RenderSystem.h"
#include "Renderer/RWindow_GLFW.h"
#include "System/IOSystem_GLFW.h"

int main(int argc, char *argv[])
{
	Game game("Limbic Engine Editor");
	auto pRender = game.GetRenderSystem();
	pRender->InitInstance("Limbic Engine Editor");

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Alek Mabry");
    QCoreApplication::setOrganizationDomain("alekstutorials.com");
    QCoreApplication::setApplicationName("Limbic Editor");

    // Create window
	IOSystem_GLFW io(game.GetApplicationName().c_str(), 640, 480);
	game.SetIO(io);
	RWindow_GLFW mainVkWindow(game.GetRenderSystem(), 640, 480, io.GetWindow(), io.GetProcess());
	mainVkWindow.CreateSurface();
	pRender->AddWindow("main", mainVkWindow);

	// Initialize Vulkan (configure device, graphics/presentation queue)
	pRender->InitGLFW();

	// Initialize RenderSystem (asset management)
	pRender->Init();

	// Initialize RWindow (swapchain, frame/depth buffers, command queues)
	mainVkWindow.Init();

	game.OnInit();
	
	EditorWindow w(&game);
	w.AttachGameWindow(&mainVkWindow);
	w.show();

    // Continued in VulkanRenderer::initResources()
    
    return a.exec();
}
