#include <QApplication>
#include <QVulkanInstance>
#include "EditorWindow.h"
#include "Game.h"
#include "System/RenderSystem.h"
#include "Renderer/RWindow.h"
#include "Renderer/RView.h"
#include "RView_Qt.h"
#include "RWindow_Qt.h"

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
    auto pVkWindow = new RWindow_Qt(&game, pRenderSystem, qVkInstance);
	pRenderSystem->AddWindow("main", *pVkWindow);

	EditorWindow w(pVkWindow, &game);
	w.show();

    // Continued in RView_Qt::initResources()
    
    return a.exec();
}
