#include "VulkanWindow.h"
#include "VulkanRenderer.h"
#include "System/RenderSystem.h"
#include "System/WorldSystem.h"
#include "Game.h"

VulkanWindow::VulkanWindow(Game* pGame, RenderSystem* pRenderSystem, QVulkanInstance& instance) : pG(pGame), pR(pRenderSystem), instance(instance), QVulkanWindow(nullptr)
{
	setVulkanInstance(&instance);
}

void VulkanWindow::Init()
{
}

void VulkanWindow::RequestUpdate()
{
	auto pW = pG->GetWorldSystem(); 
	pR->OnDrawStart();
	pW->OnDraw();

	requestUpdate();
}

RView* VulkanWindow::GetRenderView()
{
	return pV;	
}

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
	QVulkanWindow::createRenderer();

	pV = new VulkanRenderer(pG, pR, this);
	return pV;
}

VkFormat VulkanWindow::GetColorFormat() const
{
	return colorFormat();
}

int VulkanWindow::GetConcurrentFrameCount() const
{
	return concurrentFrameCount();
}

VkCommandBuffer VulkanWindow::GetCurrentCommandBuffer() const
{
	return currentCommandBuffer();
}

int VulkanWindow::GetCurrentFrame() const
{
	return currentFrame();
}

VkFramebuffer VulkanWindow::GetCurrentFramebuffer() const
{
	return currentFramebuffer();
}

int VulkanWindow::GetCurrentSwapChainImageIndex() const
{
	return currentSwapChainImageIndex();
}

VkRenderPass VulkanWindow::GetDefaultRenderPass() const
{
	return defaultRenderPass();
}

VkFormat VulkanWindow::GetDepthStencilFormat() const
{
	return depthStencilFormat();
}

VkImage VulkanWindow::GetDepthStencilImage() const
{
	return depthStencilImage();
}

VkImageView VulkanWindow::GetDepthStencilImageView() const
{
	return depthStencilImageView();
}

VkCommandPool VulkanWindow::GetGraphicsCommandPool() const
{
	return graphicsCommandPool();
}

VkQueue VulkanWindow::GetGraphicsQueue() const
{
	return graphicsQueue();
}

VkImage VulkanWindow::GetSwapChainImage(int idx) const
{
	return swapChainImage(idx);
}

int VulkanWindow::GetSwapChainImageCount() const
{
	return swapChainImageCount();
}

std::pair<int, int> VulkanWindow::GetSwapChainImageSize() const
{
	const auto size = swapChainImageSize();
	return {size.width(), size.height()};
}

VkImageView VulkanWindow::GetSwapChainImageView(int idx) const
{
	return swapChainImageView(idx);
}

VkExtent2D VulkanWindow::GetSwapChainExtent() const
{
	const auto size = swapChainImageSize();
	return VkExtent2D{(uint32_t) size.width(), (uint32_t) size.height()};
}

VkSurfaceKHR VulkanWindow::GetSurface() const
{
	return instance.surfaceForWindow(const_cast<VulkanWindow*>(this));
}
