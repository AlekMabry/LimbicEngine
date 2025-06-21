#pragma once

#include <LimbicTypes.h>

#include <vulkan/vulkan.h>
#include <eventpp/callbacklist.h>

#include <array>
#include <utility>
#include <vector>
#include <memory>

#include <Windows.h>

class RView;
class GLFWwindow;
class Game;

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
	RWindow(Game* pGame, const std::string& name, uint32 width, uint32 height);

	~RWindow();

	void Init();

	RView *GetRenderView();

	void CreateSurface();

	void DrawFrame();

	HWND GetHandle();

	/**** I/O ****/

	/*
	 * @brief Creates a mouse callback.
	 * @param callback glfwMouseButtonCallback(RWindow* window, int button, int action, int mods)
	 */
	void AddMouseButtonCallback(std::function<void(RWindow*, int, int, int)> callback);

	/*
	 * @brief Creates a mouse movement callback/
	 * @param callback glfwMouseMovementCallback(RWindow* window, double xPos, double yPos);
	 */
	void AddMousePosCallback(std::function<void(RWindow*, double, double)> callback);

	void SetMousePos(double x, double y);

	std::pair<int, int> GetWindowSize();

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
	Game* pGame;
	std::unique_ptr<RView> pV;

	HWND win32Window;
	HINSTANCE win32Process;
	const std::string name;

	uint32 width;
	uint32 height;
	GLFWwindow* window;

	eventpp::CallbackList<void(RWindow*, int, int, int)> mouseButtonEvent;
	eventpp::CallbackList<void(RWindow*, double, double)> mousePosEvent;

	friend void GLFWMouseFunc(GLFWwindow* window, int button, int action, int mods);
	friend void GLFWMousePosFunc(GLFWwindow* window, double xPos, double yPos);
};