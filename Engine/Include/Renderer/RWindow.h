#pragma once

#include <vulkan/vulkan.h>
#include <utility>
#include <vector>

class RenderSystem;
class RView;

/* Virtual class for QVulkanWindow. */
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

	/* Depth/Stencil image and swap chain framebuffer images are handled by
	 * API-specific subclasses. */

	/* Create color image for PBR forward render output, which is sampled in the postprocess subpass.
	 * Depends: Swap chain extent, image format chosen. */
	void CreateColorImage();
	void DestroyColorImage();
	VkImage subpass0_colorImage;				/// PBR Forward-render color output
	VkImageView subpass0_colorImageView;		/// PBR Forward-render color output
	VkDeviceMemory subpass0_colorImageMemory;	/// PBR Forward-render color output

	/* Create mask image for object ID/selected object.
	 * Depends: Swap chain extent, image format chosen. */
	void CreateMaskImages();
	void DestroyMaskImages();
	std::vector<VkImage> subpass0_maskImages;				/// [Editor] PBR Forward-render mask output
	std::vector<VkImageView> subpass0_maskImageViews;		/// [Editor] PBR Forward-render mask output
	std::vector<VkDeviceMemory> subpass0_maskImageMemory;	/// [Editor] PBR Forward-render mask output

	/** Depends: color, mask, depth, and present images ready. */
	void CreateFramebuffers();
	void DestroyFramebuffers();
	std::vector<VkFramebuffer> swapChainFramebuffers;

protected:
	RenderSystem* pR;
	RView* pV;
};