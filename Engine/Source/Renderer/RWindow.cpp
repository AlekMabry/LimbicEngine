#include <Renderer/RWindow.h>
#include <Renderer/RView.h>
#include <System/RenderSystem.h>

void RWindow::CreateColorImage()
{
	auto extent = GetSwapChainExtent();
	pR->CreateImage(extent.width, extent.height, GetColorFormat(), VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, subpass0_colorImage, subpass0_colorImageMemory);
	subpass0_colorImageView = pR->CreateImageView(subpass0_colorImage, GetColorFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
}

void RWindow::DestroyColorImage()
{
	vkDestroyImageView(pR->device, subpass0_colorImageView, nullptr);
	vkDestroyImage(pR->device, subpass0_colorImage, nullptr);
	vkFreeMemory(pR->device, subpass0_colorImageMemory, nullptr);
}

void RWindow::CreateMaskImages()
{
	const auto extent = GetSwapChainExtent();
	subpass0_maskImages.resize(GetSwapChainImageCount());
	subpass0_maskImageMemory.resize(GetSwapChainImageCount());
	subpass0_maskImageViews.resize(GetSwapChainImageCount());

	for (int i = 0; i < GetSwapChainImageCount(); i++)
	{
		pR->CreateImage(extent.width, extent.height, GetColorFormat(), VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, subpass0_maskImages[i], subpass0_maskImageMemory[i]);
		subpass0_maskImageViews[i] = pR->CreateImageView(subpass0_maskImages[i], GetColorFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void RWindow::DestroyMaskImages()
{
	for (int i = 0; i < GetSwapChainImageCount(); i++)
	{
		vkDestroyImageView(pR->device, subpass0_maskImageViews[i], nullptr);
		vkDestroyImage(pR->device, subpass0_maskImages[i], nullptr);
		vkFreeMemory(pR->device, subpass0_maskImageMemory[i], nullptr);
	}
}

void RWindow::CreateFramebuffers()
{
	swapChainFramebuffers.resize(GetSwapChainImageCount());
	const auto extent = GetSwapChainExtent();

	for (int i = 0; i < GetSwapChainImageCount(); i++)
	{
		std::array attachments = {subpass0_colorImageView, subpass0_maskImageViews[i],
			GetDepthStencilImageView(), GetSwapChainImageView(i)};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = pV->renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(pR->device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]));
	}
}

void RWindow::DestroyFramebuffers()
{
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(pR->device, swapChainFramebuffers[i], nullptr);
	}
}
