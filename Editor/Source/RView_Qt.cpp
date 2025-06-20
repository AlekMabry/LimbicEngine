#include <RView_Qt.h>
#include <RWindow_Qt.h>

#include <Game.h>
#include <System/RenderSystem.h>
#include <System/WorldSystem.h>

RView_Qt::RView_Qt(Game *pGame, RenderSystem* pRenderSystem, RWindow_Qt* pWindow)
	: RView(pRenderSystem, dynamic_cast<RWindow*>(pWindow), [pWindow]()
	{
		pWindow->frameReady();
		pWindow->requestUpdate(); // Render continuously, throttled by the presentation rate
	}), pG(pGame)
{
}

RView_Qt::~RView_Qt()
{
}

void RView_Qt::initSwapChainResources()
{
	auto pVW = static_cast<RWindow_Qt*>(pW);
	pVW->CreateColorImage();
	pVW->CreateMaskImages();
	InitRenderPass();
	pVW->CreateFramebuffers();
	InitGraphicsPipeline();
	InitPostprocessPipeline();
	BindMaskDescriptorSet();
}

void RView_Qt::releaseSwapChainResources()
{
	auto pVW = static_cast<RWindow_Qt*>(pW);
	pVW->DestroyMaskImages();
	pVW->DestroyFramebuffers();
	DestroyRenderPass();
}

void RView_Qt::initResources()
{
	auto pVW = dynamic_cast<QVulkanWindow*>(pW);
	pR->InitQt(pVW->device(), pVW->physicalDevice(), pVW->graphicsQueue(), pVW->graphicsQueue(), pVW->graphicsCommandPool());
	InitDescriptorPool();
	InitDescriptorSetLayout();
	InitDefaultTextureSampler();

	pR->Init();
	pG->OnInit();
}

void RView_Qt::startNextFrame()
{
	auto pW = pG->GetWorldSystem();
	pR->OnDrawStart();
	pW->OnDraw();
	StartNextFrame();
}