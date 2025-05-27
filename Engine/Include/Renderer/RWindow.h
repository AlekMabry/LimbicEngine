#pragma once

#include <vulkan/vulkan.h>
#include <utility>

class RView;

/** Virtual class for QVulkanWindow. */
class RWindow
{
public:
	/* Render system must be initialized first. */
	virtual void Init() = 0;

	/* Triggers RView render. */
	virtual void RequestUpdate() = 0;

	/* Null until initialized. */
	virtual RView *GetRenderView() = 0;

	/**** QVulkanWindow interface. ****/

	virtual VkFormat GetColorFormat() const = 0;
	virtual int GetConcurrentFrameCount() const = 0;
	virtual VkCommandBuffer GetCurrentCommandBuffer() const = 0;
	virtual int GetCurrentFrame() const = 0;
	virtual VkFramebuffer GetCurrentFramebuffer() const = 0;
	virtual int GetCurrentSwapChainImageIndex() const = 0;
	virtual VkRenderPass GetDefaultRenderPass() const = 0;
	virtual VkFormat GetDepthStencilFormat() const = 0;
	virtual VkImage GetDepthStencilImage() const = 0;
	virtual VkImageView GetDepthStencilImageView() const = 0;
	virtual VkCommandPool GetGraphicsCommandPool() const = 0;
	virtual VkQueue GetGraphicsQueue() const = 0;
	virtual VkImage GetSwapChainImage(int idx) const = 0;
	virtual int GetSwapChainImageCount() const = 0;
	virtual std::pair<int, int> GetSwapChainImageSize() const = 0;
	virtual VkImageView GetSwapChainImageView(int idx) const = 0;
	virtual VkExtent2D GetSwapChainExtent() const = 0;
	virtual VkSurfaceKHR GetSurface() const = 0;
};