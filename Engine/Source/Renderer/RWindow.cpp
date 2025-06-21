#include <System/RenderSystem.h>
#include <Renderer/RWindow.h>
#include <Renderer/RView.h>

RWindow::RWindow(RenderSystem* pRenderSystem, uint32 width, uint32 height, HWND window, HINSTANCE process)
	: pR(pRenderSystem), width(width), height(height), win32Window(window), win32Process(process)
{
}

RWindow::~RWindow()
{
}

void RWindow::Init()
{
	pV = std::make_unique<RView>(pR, this, [this]() { RWindow::DrawFrame(); });
	CreateSurface();
	CreateSwapchain();
	CreateRenderPass();
	pV->InitDescriptorPool();
	pV->InitDescriptorSetLayout();
	pV->InitGraphicsPipeline();
	CreateDepthbuffer();
	CreateFramebuffers();
	pV->InitGraphicsCommandPool();
	CreateCommandBuffers();
	pV->InitDefaultTextureSampler();
	CreateSyncObjects();
}

RView* RWindow::GetRenderView()
{
	return pV.get();
}

void RWindow::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = win32Window;
	createInfo.hinstance = win32Process;

	VK_CHECK(vkCreateWin32SurfaceKHR(pR->instance, &createInfo, nullptr, &surface));
}

void RWindow::CreateCommandBuffers()
{
	for (auto& submissionResource : submissionResources)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = pR->commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VK_CHECK(vkAllocateCommandBuffers(pR->device, &allocInfo, &submissionResource.commandBuffer));
	}
}

void RWindow::CreateSwapchain()
{
	SSwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(pR->physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = PickSwapSurfaceFormat(swapChainSupport.formats);
	swapchainImageFormat = surfaceFormat.format;
	VkPresentModeKHR presentMode = PickSwapPresentMode(swapChainSupport.presentModes);
	swapchainExtent = PickSwapExtent(swapChainSupport.capabilities);

	uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchainExtent;
	swapchainCreateInfo.imageArrayLayers = 1;	 // For VR this may need to be increased
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	SQueueFamilyIndices indices = pR->FindQueueFamilies(pR->physicalDevice);
	uint32 queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(pR->device, &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create swap chain!");
	}

	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
	};

	std::vector<VkImage> swapchainImages;
	vkGetSwapchainImagesKHR(pR->device, swapchain, &imageCount, nullptr);
	swapchainResources.resize(imageCount);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(pR->device, swapchain, &imageCount, swapchainImages.data());
	for (uint32 i = 0; i < imageCount; i++)
	{
		swapchainResources[i].presentImage = swapchainImages[i];
		swapchainResources[i].presentImageView = pR->CreateImageView(swapchainResources[i].presentImage, swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		VK_CHECK(vkCreateSemaphore(pR->device, &semaphoreCreateInfo, nullptr, &swapchainResources[i].drawCompleteSemaphore));
	}
}

void RWindow::CreateDepthbuffer()
{
	depthImageFormat = pR->FindDepthFormat();
	pR->CreateImage(swapchainExtent.width, swapchainExtent.height, depthImageFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = pR->CreateImageView(depthImage, depthImageFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void RWindow::CreateFramebuffers()
{
	for (auto& swapchainResource : swapchainResources)
	{
		std::array attachments = {swapchainResource.presentImageView, depthImageView};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchainExtent.width;
		framebufferInfo.height = swapchainExtent.height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(pR->device, &framebufferInfo, nullptr, &swapchainResource.framebuffer));
	}
}

void RWindow::DestroySwapchain()
{
	vkDestroyImageView(pR->device, depthImageView, nullptr);
	vkDestroyImage(pR->device, depthImage, nullptr);
	vkFreeMemory(pR->device, depthImageMemory, nullptr);

	for (auto& swapchainResource : swapchainResources)
	{
		vkDestroyFramebuffer(pR->device, swapchainResource.framebuffer, nullptr);
		vkDestroyImageView(pR->device, swapchainResource.presentImageView, nullptr);
	}

	vkDestroySwapchainKHR(pR->device, swapchain, nullptr);
}

void RWindow::CreateSyncObjects()
{
	// Submission sync objects
	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
	};

	VkFenceCreateInfo fenceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT
	};

	for (auto& submissionResource : submissionResources)
	{
		VK_CHECK(vkCreateFence(pR->device, &fenceCreateInfo, nullptr, &submissionResource.fence));
		VK_CHECK(vkCreateSemaphore(pR->device, &semaphoreCreateInfo, nullptr, &submissionResource.imageAcquiredSemaphore));
	}
}

void RWindow::RecreateSwapchain()
{
	vkDeviceWaitIdle(pR->device);
	DestroySwapchain();
	CreateSwapchain();
	CreateDepthbuffer();
	CreateFramebuffers();
}

void RWindow::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapchainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;	 // Clear framebuffer
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = pR->FindDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array attachments = {colorAttachment, depthAttachment};

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_CHECK(vkCreateRenderPass(pR->device, &renderPassInfo, nullptr, &renderPass));
}

void RWindow::DrawFrame()
{
	auto& currentSubmission = submissionResources[currentSubmissionIndex];

	// Ensure no more than FRAME_LAG renderings are outstanding
	vkWaitForFences(pR->device, 1, &currentSubmission.fence, VK_TRUE, UINT64_MAX);

	uint32 currentSwapchainImageIndex;
	VkResult result;
	do
	{
		result = vkAcquireNextImageKHR(pR->device, swapchain, UINT64_MAX, currentSubmission.imageAcquiredSemaphore,
			VK_NULL_HANDLE, &currentSwapchainImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
		}
		else if (result == VK_SUBOPTIMAL_KHR)
		{
			// Non-optimal but still presentable
			break;
		}
		else if (result == VK_ERROR_SURFACE_LOST_KHR)
		{
			vkDestroySurfaceKHR(pR->instance, surface, nullptr);
			CreateSurface();
			// Resize;
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] Failed to acquire next presentation image.");
		}
	} while (result != VK_SUCCESS);

	auto& currentSwapchainResource = swapchainResources[currentSwapchainImageIndex];

	VK_CHECK(vkResetCommandBuffer(currentSubmission.commandBuffer, 0));

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	VK_CHECK(vkBeginCommandBuffer(currentSubmission.commandBuffer, &beginInfo));
	pV->RecordCommandBuffer(currentSubmission.commandBuffer, currentSwapchainResource.framebuffer, swapchainExtent);

	VK_CHECK(vkEndCommandBuffer(currentSubmission.commandBuffer));

	// if separate present queue, build ownership queue

	vkResetFences(pR->device, 1, &currentSubmission.fence);

	// Wait for image acquired semaphore to be signaled to ensure that the
	// image won't be rendered to until the presentation engine has fully
	// released ownership to the application.
	VkPipelineStageFlags pipelineStateFlags;
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	pipelineStateFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submitInfo.pWaitDstStageMask = &pipelineStateFlags;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &currentSubmission.imageAcquiredSemaphore;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &currentSubmission.commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &currentSwapchainResource.drawCompleteSemaphore;
	VK_CHECK(vkQueueSubmit(pR->graphicsQueue, 1, &submitInfo, currentSubmission.fence));

	VkPresentInfoKHR present = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &currentSwapchainResource.drawCompleteSemaphore,
		.swapchainCount = 1,
		.pSwapchains = &swapchain,
		.pImageIndices = &currentSwapchainImageIndex,
	};

	VkResult presentResult = vkQueuePresentKHR(pR->presentQueue, &present);
	currentSubmissionIndex++;
	currentSubmissionIndex %= FRAME_LAG;
	bFirstSwapchainFrame = false;

	if (presentResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapchain();
	}
	else if (presentResult == VK_SUBOPTIMAL_KHR)
	{
		// SUBOPTIMAL could be due to a resize
		VkSurfaceCapabilitiesKHR surfCapabilities;
		VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pR->physicalDevice, surface, &surfCapabilities));
		if (surfCapabilities.currentExtent.width != swapchainExtent.width ||
			surfCapabilities.currentExtent.height != swapchainExtent.height)
		{
			width = surfCapabilities.currentExtent.width;
			height = surfCapabilities.currentExtent.height;
			RecreateSwapchain();
		}
	}
	else if (presentResult == VK_ERROR_SURFACE_LOST_KHR)
	{
		vkDestroySurfaceKHR(pR->instance, surface, nullptr);
		CreateSurface();
		RecreateSwapchain();
	}
	else if (presentResult != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to present.");
	}
}

HWND RWindow::GetHandle()
{
	return win32Window;
}

VkSurfaceFormatKHR RWindow::PickSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR RWindow::PickSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RWindow::PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = {width, height};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height =
			std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

SSwapChainSupportDetails RWindow::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SSwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}