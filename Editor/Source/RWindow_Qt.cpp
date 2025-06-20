#include "RWindow_Qt.h"
#include "RView_Qt.h"
#include "System/RenderSystem.h"
#include "System/WorldSystem.h"
#include "Game.h"

RWindow_Qt::RWindow_Qt(Game* pGame, RenderSystem* pRenderSystem, QVulkanInstance& instance)
	: pG(pGame), instance(instance), QVulkanWindow(nullptr)
{
	setVulkanInstance(&instance);
	pR = pRenderSystem;
}

void RWindow_Qt::Init()
{
}

void RWindow_Qt::RequestUpdate()
{
	auto pW = pG->GetWorldSystem(); 
	pR->OnDrawStart();
	pW->OnDraw();

	requestUpdate();
}

RView* RWindow_Qt::GetRenderView()
{
	return pV;	
}

QVulkanWindowRenderer* RWindow_Qt::createRenderer()
{
	QVulkanWindow::createRenderer();

	auto pVQt = new RView_Qt(pG, pR, this);
	pV = static_cast<RView*>(pVQt);
	return pVQt;
}

VkFormat RWindow_Qt::GetColorFormat() const
{
	return colorFormat();
}

int RWindow_Qt::GetConcurrentFrameCount() const
{
	return concurrentFrameCount();
}

VkCommandBuffer RWindow_Qt::GetCurrentCommandBuffer() const
{
	return currentCommandBuffer();
}

int RWindow_Qt::GetCurrentFrame() const
{
	return currentFrame();
}

VkFramebuffer RWindow_Qt::GetCurrentFramebuffer() const
{
	return swapChainFramebuffers[currentFrame()];
}

int RWindow_Qt::GetCurrentSwapChainImageIndex() const
{
	return currentSwapChainImageIndex();
}

VkFormat RWindow_Qt::GetDepthStencilFormat() const
{
	return depthStencilFormat();
}

VkImage RWindow_Qt::GetDepthStencilImage() const
{
	return depthStencilImage();
}

VkImageView RWindow_Qt::GetDepthStencilImageView() const
{
	return depthStencilImageView();
}

VkCommandPool RWindow_Qt::GetGraphicsCommandPool() const
{
	return graphicsCommandPool();
}

VkQueue RWindow_Qt::GetGraphicsQueue() const
{
	return graphicsQueue();
}

VkImage RWindow_Qt::GetSwapChainImage(int idx) const
{
	return swapChainImage(idx);
}

int RWindow_Qt::GetSwapChainImageCount() const
{
	return swapChainImageCount();
}

std::pair<int, int> RWindow_Qt::GetSwapChainImageSize() const
{
	const auto size = swapChainImageSize();
	return {size.width(), size.height()};
}

VkImageView RWindow_Qt::GetSwapChainImageView(int idx) const
{
	return swapChainImageView(idx);
}

VkExtent2D RWindow_Qt::GetSwapChainExtent() const
{
	const auto size = swapChainImageSize();
	return VkExtent2D{(uint32_t) size.width(), (uint32_t) size.height()};
}

VkSurfaceKHR RWindow_Qt::GetSurface() const
{
	return instance.surfaceForWindow(const_cast<RWindow_Qt*>(this));
}