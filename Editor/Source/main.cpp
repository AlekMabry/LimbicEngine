#include <QApplication>
#include <QVulkanInstance>
#include "EditorWindow.h"
#include "Game.h"
#include "System/RenderSystem.h"
#include "Renderer/RWindow.h"
#include "Renderer/RView.h"
#include "VulkanRenderer.h"
#include "VulkanWindow.h"

int main(int argc, char *argv[])
{
	Game game("Limbic Engine Editor");
    auto pRenderSystem = game.GetRenderSystem();
	auto vkInstance = pRenderSystem->InitInstance("Limbic Engine Editor");

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Alek Mabry");
    QCoreApplication::setOrganizationDomain("alekstutorials.com");
    QCoreApplication::setApplicationName("Limbic Editor");

    QVulkanInstance qVkInstance;
	qVkInstance.setVkInstance(vkInstance);
	qVkInstance.setLayers({"VK_LAYER_KHRONOS_validation"});
    qVkInstance.create();
    auto pVkWindow = new VulkanWindow(&game, pRenderSystem, qVkInstance);
	pRenderSystem->AddWindow("main", *pVkWindow);

	EditorWindow w(pVkWindow, &game);
	w.show();

    // Continued in VulkanRenderer::initResources()
    
    return a.exec();
}
