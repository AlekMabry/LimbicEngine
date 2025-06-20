#pragma once

#include "LimbicTypes.h"
#include "Renderer/RWindow.h"

#include <Windows.h>
#include <vector>
#include <vulkan/vulkan.hpp>

const uint32 MAX_FRAMES_IN_FLIGHT = 2;

struct SSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class RenderSystem;

class RWindow_GLFW : public RWindow
{
public:
	RWindow_GLFW(RenderSystem *pRenderSystem, uint32 width, uint32 height, HWND window, HINSTANCE process);
	~RWindow_GLFW();
	void Init() override;
	void RequestUpdate() override;
	RView* GetRenderView() override;

	/**** QVulkanWindow interface. ****/

	VkFormat GetColorFormat() const override;
	int GetConcurrentFrameCount() const override;
	VkCommandBuffer GetCurrentCommandBuffer() const override;
	int GetCurrentFrame() const override;
	VkFramebuffer GetCurrentFramebuffer() const override;
	int GetCurrentSwapChainImageIndex() const override;
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

	/**** Initialize/destroy class members. ****/

	void InitSurface();
	void InitCommandBuffers();
	void InitSwapChain();
	void InitDepthbuffer();
	void DestroySwapChain();	// Todo - Integrate DestroyFramebuffers();
	void InitSyncObjects();
	void RecreateSwapChain();

	/**** Option picking utilities. ****/

	VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	/**** Option finding utitilies. ****/

	SSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	RenderSystem *pR;
	std::unique_ptr<RView> pV;

	HWND win32Window;
	HINSTANCE win32Process;

	uint32 width;
	uint32 height;
	
	VkSurfaceKHR surface;

	// Sync
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	uint32 imageIndex;
	uint32 currentFrame = 0;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkImage depthImage;
	VkFormat depthImageFormat;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;
	VkImage maskImage;
	VkImageView maskImageView;
	VkDeviceMemory maskImageMemory;

private:
	void PresentFrame();
};
