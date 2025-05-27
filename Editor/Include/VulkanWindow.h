#pragma once

#include "Renderer/RWindow.h"

#include <QVulkanWindow>

class Game;
class RenderSystem;
class VulkanRenderer;

class VulkanWindow : public QVulkanWindow, public RWindow
{
public:
	VulkanWindow(Game *pGame, RenderSystem *pRenderSystem, QVulkanInstance &instance);
	void Init() override;
	void RequestUpdate() override;
	RView* GetRenderView() override;
	
	QVulkanWindowRenderer *createRenderer() override;

	VkFormat GetColorFormat() const override;
	int GetConcurrentFrameCount() const override;
	VkCommandBuffer GetCurrentCommandBuffer() const override;
	int GetCurrentFrame() const override;
	VkFramebuffer GetCurrentFramebuffer() const override;
	int GetCurrentSwapChainImageIndex() const override;
	VkRenderPass GetDefaultRenderPass() const override;
	VkFormat GetDepthStencilFormat() const override;
	VkImage GetDepthStencilImage() const override;
	VkImageView GetDepthStencilImageView() const override;
	VkCommandPool GetGraphicsCommandPool() const override;
	VkQueue GetGraphicsQueue() const override;
	VkImage GetSwapChainImage(int idx) const override;
	int GetSwapChainImageCount() const override;
	std::pair<int, int> GetSwapChainImageSize() const override;
	VkImageView GetSwapChainImageView(int idx) const override;
	VkExtent2D GetSwapChainExtent() const override;
	VkSurfaceKHR GetSurface() const override;

private:
	QVulkanInstance &instance;
	VulkanRenderer* pV;	// Render View
	RenderSystem* pR;
	Game* pG;
};
