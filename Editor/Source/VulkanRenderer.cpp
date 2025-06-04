#include <VulkanRenderer.h>
#include <VulkanWindow.h>

#include <Game.h>
#include <System/RenderSystem.h>
#include <System/WorldSystem.h>

VulkanRenderer::VulkanRenderer(Game *pGame, RenderSystem* pRenderSystem, QVulkanWindow* pWindow)
	: RView(pRenderSystem, dynamic_cast<RWindow*>(pWindow), [pWindow]()
	{
		pWindow->frameReady();
		pWindow->requestUpdate(); // Render continuously, throttled by the presentation rate
	}), pG(pGame)
{
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::initResources()
{
	auto pVW = dynamic_cast<QVulkanWindow*>(pW);
	pR->InitQt(pVW->device(), pVW->physicalDevice(), pVW->graphicsQueue(), pVW->graphicsQueue(), pVW->graphicsCommandPool());
	InitDescriptorPool();
	InitDescriptorSetLayout();
	InitGraphicsPipeline();
	InitDefaultTextureSampler();

	pR->Init();
	pG->OnInit();
}

void VulkanRenderer::startNextFrame()
{
	auto pW = pG->GetWorldSystem();
	pR->OnDrawStart();
	pW->OnDraw();
	StartNextFrame();
}