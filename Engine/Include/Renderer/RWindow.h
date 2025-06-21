#pragma once

#include <LimbicTypes.h>

#include <vulkan/vulkan.h>

#include <array>
#include <utility>
#include <vector>
#include <memory>

#include <Windows.h>

class RView;

struct SSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct SSubmissionResources
{
	/* Throttle if we get too far ahead of image presents. */
	VkFence fence;

	/* VkAcquireNextImageKHR wait. */
	VkSemaphore imageAcquiredSemaphore;

	VkCommandBuffer commandBuffer;
};

struct SSwapchainImageResources
{
	VkImage pbrColorImage;
	VkImageView pbrColorImageView;
	VkImage presentImage;
	VkImageView presentImageView;
	VkFramebuffer framebuffer;
	VkSemaphore drawCompleteSemaphore;		/// Signaled by completed draw call
};

#define FRAME_LAG 3

/** Virtual class for QVulkanWindow. */
class RWindow
{
	friend class RenderSystem;
	friend class RView;

public:
	RWindow(RenderSystem* pRenderSystem, uint32 width, uint32 height, HWND window, HINSTANCE process);

	~RWindow();

	void Init();

	RView *GetRenderView();

	void CreateSurface();

	void DrawFrame();

	HWND GetHandle();

protected:
	/**** Initialize/destroy class members. ****/

	void CreateCommandBuffers();

	void CreateSwapchain();

	void CreateDepthbuffer();

	void CreateFramebuffers();

	void DestroySwapchain();

	void CreateSyncObjects();

	void DestroySyncObjects();

	void RecreateSwapchain();

	void CreateRenderPass();

	/**** Option picking utilities. ****/

	VkSurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR PickSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	/**** Option finding utitilies. ****/

	SSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceKHR surface;
	VkRenderPass renderPass;

	VkSwapchainKHR swapchain;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;
	std::vector<SSwapchainImageResources> swapchainResources;
	bool bFirstSwapchainFrame = false;

	VkImage depthImage;
	VkFormat depthImageFormat;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;

	std::array<SSubmissionResources, FRAME_LAG> submissionResources;
	uint32 currentSubmissionIndex = 0;

	RenderSystem* pR;
	std::unique_ptr<RView> pV;

	HWND win32Window;
	HINSTANCE win32Process;

	uint32 width;
	uint32 height;
};