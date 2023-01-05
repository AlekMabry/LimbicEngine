#include "RenderSystem.h"

/**** Public interface. ****/

RenderSystem::~RenderSystem()
{
	DestroySwapChain();

	vkDestroySampler(device, textureSampler, nullptr);
	vkDestroyDescriptorPool(device, descriptorPool_PBR, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout_PBR, nullptr);

	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

	vkDestroyRenderPass(device, renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(device, commandPool, nullptr);

	if (bEnableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void RenderSystem::Init(const char* applicationName, IOSystem* pIO)
{
	this->applicationName = applicationName;
	this->pIO = pIO;
	currentFrame = 0;

	std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	PickPhysicalDevice(deviceExtensions);
	InitDevice(deviceExtensions);
	PickDeviceMemoryBlockTypes();
	InitSwapChain();
	InitRenderPass();
	InitDescriptorPool();
	InitDescriptorSetLayout();
	InitGraphicsPipeline();
	InitDepthbuffer();
	InitSwapChainFramebuffers();
	InitGraphicsCommandPool();
	InitDeviceMemoryManager();
	InitCommandBuffers();
	InitSyncObjects();
	InitDefaultTextureSampler();
}

void RenderSystem::CreateStaticMesh(
	uint32 vertices, uint32 indices, uint32& meshHandle, SStaticVertex*& vertexBuffer, uint32*& indexBuffer)
{
	SMeshMemoryHandle mesh;
	mesh.location = eMemoryLocationHost;
	VkDeviceSize vertexSize = (vertices * sizeof(SStaticVertex));
	VkDeviceSize indexSize = (indices * sizeof(uint32));

	// Todo - pick a block with enough memory.
	SMemoryBlock* stage = &stagingMemory[0];
	mesh.vertexBlock = 0;
	mesh.indexBlock = 0;

	mesh.vertexOffset = stage->cursor;
	mesh.vertices = vertices;
	vertexBuffer = reinterpret_cast<SStaticVertex*>((uint8*) stage->mappedLocation + stage->cursor);
	stage->cursor += vertexSize;

	mesh.indexOffset = stage->cursor;
	mesh.indices = indices;
	indexBuffer = reinterpret_cast<uint32*>((uint8*) stage->mappedLocation + stage->cursor);
	stage->cursor += indexSize;

	meshHandle = static_cast<uint32>(meshes.size());
	meshesInStagingMemory.push_back(static_cast<uint32>(meshes.size()));
	meshes.push_back(mesh);
}

void RenderSystem::CreateTexture(uint32 width, uint32 height, ETextureFormat format, uint32& textureHandle, void*& textureBuffer)
{
	STextureMemoryHandle texture;
	texture.width = width;
	texture.height = height;
	texture.format = format;
	switch (format)
	{
		case eTextureFormatRGBA:
			texture.size = width * height * 4;
			break;
		case eTextureFormatDXT1:
			texture.size = (width * height) / 2;
			break;
	}

	// Todo - pick a block with enough memory.
	SMemoryBlock* stage = &stagingMemory[1];
	texture.block = 1;
	texture.location = eMemoryLocationHost;

	texture.offset = stage->cursor;
	textureBuffer = reinterpret_cast<void*>((uint8*) stage->mappedLocation + stage->cursor);
	stage->cursor += texture.size;

	texturesInStagingMemory.push_back(static_cast<uint32>(textures.size()));
	textureHandle = static_cast<uint32>(textures.size());
	textures.push_back(texture);
}

void RenderSystem::CreateMaterial(RTexture baseColor, RTexture normal, RTexture properties, RMaterial& material)
{
	SMaterialHandle mat;
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool_PBR;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout_PBR;

	VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, &mat.descriptor));

	VkDescriptorImageInfo baseColorInfo{};
	baseColorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	baseColorInfo.imageView = textures[baseColor].imageView;
	baseColorInfo.sampler = textureSampler;

	VkDescriptorImageInfo normalInfo{};
	normalInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	normalInfo.imageView = textures[normal].imageView;
	normalInfo.sampler = textureSampler;

	VkDescriptorImageInfo propertiesInfo{};
	propertiesInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	propertiesInfo.imageView = textures[properties].imageView;
	propertiesInfo.sampler = textureSampler;

	std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = mat.descriptor;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &baseColorInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = mat.descriptor;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &normalInfo;

	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[2].dstSet = mat.descriptor;
	descriptorWrites[2].dstBinding = 2;
	descriptorWrites[2].dstArrayElement = 0;
	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[2].descriptorCount = 1;
	descriptorWrites[2].pImageInfo = &propertiesInfo;

	vkUpdateDescriptorSets(device, static_cast<uint32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	material = static_cast<RMaterial>(materials.size());
	materials.push_back(mat);
}

void RenderSystem::SubmitAssets()
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	for (uint32 meshIndex : meshesInStagingMemory)
	{
		VkBufferCopy copyVertexRegion, copyIndexRegion;
		uint32 vertexBlock, indexBlock;

		copyVertexRegion.size = meshes[meshIndex].vertices * sizeof(SStaticVertex);
		copyVertexRegion.srcOffset = meshes[meshIndex].vertexOffset;
		DeviceMalloc(copyVertexRegion.size, eMemoryBlockUsageGeometry, copyVertexRegion.dstOffset, vertexBlock);
		vkCmdCopyBuffer(commandBuffer, stagingMemory[meshes[meshIndex].vertexBlock].buffer, deviceMemory[vertexBlock].buffer, 1,
			&copyVertexRegion);

		copyIndexRegion.size = meshes[meshIndex].indices * sizeof(uint32);
		copyIndexRegion.srcOffset = meshes[meshIndex].indexOffset;
		DeviceMalloc(copyIndexRegion.size, eMemoryBlockUsageGeometry, copyIndexRegion.dstOffset, indexBlock);
		vkCmdCopyBuffer(commandBuffer, stagingMemory[meshes[meshIndex].indexBlock].buffer, deviceMemory[indexBlock].buffer, 1,
			&copyIndexRegion);

		// Update mesh indexing to new device memory location
		meshes[meshIndex].location = eMemoryLocationHost;
		meshes[meshIndex].vertexBlock = vertexBlock;
		meshes[meshIndex].indexBlock = indexBlock;
		meshes[meshIndex].vertexOffset = copyVertexRegion.dstOffset;
		meshes[meshIndex].indexOffset = copyIndexRegion.dstOffset;
	}

	for (uint32 textureIndex : texturesInStagingMemory)
	{
		STextureMemoryHandle* texture = &textures[textureIndex];
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = texture->width;
		imageInfo.extent.height = texture->height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = textureFormatVkFormat[texture->format];
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = memoryBlockUsageVkFlags[eMemoryBlockUsageImages];
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &texture->image));
		
		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(device, texture->image, &memoryRequirements);
		deviceDataAlignment[eMemoryBlockUsageImages] = memoryRequirements.alignment;	// This should be passed to the DeviceMalloc

		VkImageMemoryBarrier stagingBarrier{}, shaderBarrier{};
		CreateTextureMemoryBarrier(
			texture->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, stagingBarrier);
		CreateTextureMemoryBarrier(texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, shaderBarrier);

		VkDeviceSize imageDstOffset;
		uint32 deviceBlock;
		DeviceMalloc(memoryRequirements.size, eMemoryBlockUsageImages, imageDstOffset, deviceBlock);
		vkBindImageMemory(device, texture->image, deviceMemory[deviceBlock].memory, imageDstOffset);

		texture->imageView = CreateImageView(texture->image, textureFormatVkFormat[texture->format], VK_IMAGE_ASPECT_COLOR_BIT);

		VkBufferImageCopy region{};
		region.bufferOffset = texture->offset;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {texture->width, texture->height, 1};

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
			nullptr, 1, &stagingBarrier);
		vkCmdCopyBufferToImage(
			commandBuffer, stagingMemory[texture->block].buffer, texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
			nullptr, 1, &shaderBarrier);

		// Update texture indexing to new device memory location
		texture->location = eMemoryLocationDevice;
		texture->offset = 0;
		texture->size = memoryRequirements.size;
		texture->block = deviceBlock;
	}

	EndSingleTimeCommands(commandBuffer);
	meshesInStagingMemory.clear();
	texturesInStagingMemory.clear();
	stagingMemory[0].cursor = 0;
	stagingMemory[1].cursor = 0;
}

void RenderSystem::OnDrawStart()
{
	drawStaticCommands.clear();
}

void RenderSystem::OnDrawEnd()
{
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32 imageIndex;
	VkResult acquireResult =
		vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("[ERROR] Failed to acquire swap chain image!");
	}

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult presentResult = vkQueuePresentKHR(presentQueue, &presentInfo);
	if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR || framebufferResized)
	{
		framebufferResized = false;
		RecreateSwapChain();
	}
	else if (presentResult != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderSystem::DrawStaticMesh(RStaticMesh meshId, RMaterial materialId, mat4 modelTransform)
{
	SDrawStaticPBR drawCommand = {meshId, materialId, modelTransform};
	drawStaticCommands.push_back(drawCommand);
}

void RenderSystem::DrawSetCamera(mat4 transform)
{
	camera = transform;
}

/**** Option finding utilities. ****/

uint32 RenderSystem::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32 i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("[ERROR] Failed to find suitable memory type!");
}

VkFormat RenderSystem::FindSupportedFormat(
	const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("[ERROR] Failed to find supported format!");
}

VkFormat RenderSystem::FindDepthFormat()
{
	return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
		VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


/**** Initialize/destroy class members. *****/

void RenderSystem::DestroySwapChain()
{
	vkDestroyImageView(device, depthImageView, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);

	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void RenderSystem::InitDescriptorPool()
{
	VkDescriptorPoolSize poolSizes[3];
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = 16;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 16;
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[2].descriptorCount = 16;

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 3;
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = 16;

	VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool_PBR));
}

void RenderSystem::InitDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding bindings[3];

	bindings[0].binding = 0;
	bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings[0].descriptorCount = 1;
	bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[0].pImmutableSamplers = nullptr;

	bindings[1].binding = 1;
	bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings[1].descriptorCount = 1;
	bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[1].pImmutableSamplers = nullptr;

	bindings[2].binding = 2;
	bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings[2].descriptorCount = 1;
	bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[2].pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 3;
	layoutInfo.pBindings = bindings;

	VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout_PBR));
}

void RenderSystem::InitGraphicsPipeline()
{
	auto vertShaderCode = LoadSPIRV("C:/Users/alekm/Desktop/Outpost731/Shaders/staticPBRVert.spv");
	auto fragShaderCode = LoadSPIRV("C:/Users/alekm/Desktop/Outpost731/Shaders/staticPBRFrag.spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	// vertShaderStageInfo.pSpecializationInfo - Important in future

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	// fragShaderStageInfo.pSpecializationInfo - Important in future

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(SStaticVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(SStaticVertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(SStaticVertex, normal);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(SStaticVertex, tangent);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(SStaticVertex, bitangent);

	attributeDescriptions[4].binding = 0;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(SStaticVertex, textureUV);

	attributeDescriptions[5].binding = 0;
	attributeDescriptions[5].location = 5;
	attributeDescriptions[5].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[5].offset = offsetof(SStaticVertex, lightmapUV);

	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32>(attributeDescriptions.size());
	vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapChainExtent.width;
	viewport.height = (float) swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;

	std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo{};
	rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerCreateInfo.lineWidth = 1.0f;
	rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizerCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizerCreateInfo.depthBiasClamp = 0.0f;
	rasterizerCreateInfo.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {};
	depthStencil.back = {};

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SPushConstants);
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_PBR;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout));

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputCreateInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;	// Used to derive new pipeline from existing

	VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void RenderSystem::InitRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;	 // Clear framebuffer
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();
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

	std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create render pass!");
	}
}

void RenderSystem::InitSwapChainFramebuffers()
{
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {swapChainImageViews[i], depthImageView};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] Failed to create framebuffer!");
		}
	}
}

void RenderSystem::InitDepthbuffer()
{
	VkFormat depthFormat = FindDepthFormat();
	CreateImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void RenderSystem::InitGraphicsCommandPool()
{
	SQueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create command pool!");
	}
}

void RenderSystem::InitCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to allocate command buffers!");
	}
}

void RenderSystem::InitSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] Failed to create sync objects for a frame!");
		}
	}
}

void RenderSystem::InitDeviceMemoryManager()
{
	CreateDeviceMemoryBlock(eMemoryBlockUsageGeometry);
	CreateDeviceMemoryBlock(eMemoryBlockUsageImages);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, deviceMemory[0].buffer, &memRequirements);
	deviceDataAlignment[eMemoryBlockUsageGeometry] = memRequirements.alignment;

	SMemoryBlock stagingBlock{};
	CreateBuffer(MEMORY_BLOCK_SIZE, eMemoryBlockUsageStaging,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBlock.buffer, stagingBlock.memory);
	vkMapMemory(device, stagingBlock.memory, 0, MEMORY_BLOCK_SIZE, 0, &stagingBlock.mappedLocation);
	stagingMemory.push_back(stagingBlock);

	SMemoryBlock stagingBlockB{};
	CreateBuffer(MEMORY_BLOCK_SIZE, eMemoryBlockUsageStaging,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBlockB.buffer, stagingBlockB.memory);
	vkMapMemory(device, stagingBlockB.memory, 0, MEMORY_BLOCK_SIZE, 0, &stagingBlockB.mappedLocation);
	stagingMemory.push_back(stagingBlockB);
}

void RenderSystem::DestroyDeviceMemoryManager()
{
	for (SMemoryBlock block : deviceMemory)
	{
		vkDestroyBuffer(device, block.buffer, nullptr);
		vkFreeMemory(device, block.memory, nullptr);
	}

	for (SMemoryBlock block : stagingMemory)
	{
		vkUnmapMemory(device, block.memory);
		vkDestroyBuffer(device, block.buffer, nullptr);
		vkFreeMemory(device, block.memory, nullptr);
	}
}

void RenderSystem::InitDefaultTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	// VkPhysicalDeviceProperties properties{};
	// vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	// samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	VK_CHECK(vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler));
}

/**** Utility. ****/

uint32 RenderSystem::CreateDeviceMemoryBlock(EMemoryBlockUsage usage)
{
	SMemoryBlock deviceBlock{};
	deviceBlock.usage = usage;
	switch (usage)
	{
		case eMemoryBlockUsageGeometry:
			CreateBuffer(MEMORY_BLOCK_SIZE, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceBlock.buffer,
				deviceBlock.memory);
			break;
		case eMemoryBlockUsageUniforms:
			CreateBuffer(MEMORY_BLOCK_SIZE, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceBlock.buffer,
				deviceBlock.memory);
			break;
		case eMemoryBlockUsageImages:
			AllocateDeviceMemory(MEMORY_BLOCK_SIZE, memoryBlockUsageLocation[usage], deviceBlock.memory);
			break;
		case eMemoryBlockUsageStaging:
			break;
	}
	deviceMemory.push_back(deviceBlock);
	return static_cast<uint32>(deviceMemory.size() - 1);
}

void RenderSystem::DeviceMalloc(VkDeviceSize size, EMemoryBlockUsage usage, VkDeviceSize& offset, uint32& block)
{
	for (size_t i = 0; i < deviceMemory.size(); i++)
	{
		if (usage != deviceMemory[i].usage)
			continue;

		VkDeviceSize alignment = deviceDataAlignment[usage];

		size_t potentialOffset = deviceMemory[i].cursor % alignment == 0
									 ? deviceMemory[i].cursor
									 : deviceMemory[i].cursor + (alignment - (deviceMemory[i].cursor % alignment));
		size_t available = MEMORY_BLOCK_SIZE - potentialOffset - 1;

		if (available >= size)
		{
			block = static_cast<uint32>(i);
			offset = potentialOffset;
			deviceMemory[i].cursor = potentialOffset + size;
			return;
		}
	}

	block = static_cast<uint32>(deviceMemory.size());
	offset = 0;
	deviceMemory[block].cursor += size;

	CreateDeviceMemoryBlock(usage);
}

void RenderSystem::AllocateDeviceMemory(VkDeviceSize size, EMemoryLocation location, VkDeviceMemory& deviceMemory)
{
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = size;
	allocInfo.memoryTypeIndex = deviceMemoryTypeByLocation[location];

	if (vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to allocate device memory!");
	}
}

void RenderSystem::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to allocate image memory!");
	}

	vkBindImageMemory(device, image, imageMemory, 0);
}

void RenderSystem::CreateBuffer(
	VkDeviceSize size, EMemoryBlockUsage usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = memoryBlockUsageVkFlags[usage];
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create buffer!");
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);
	AllocateDeviceMemory(memoryRequirements.size, memoryBlockUsageLocation[usage], bufferMemory);
	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void RenderSystem::CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}

void RenderSystem::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32 width, uint32 height)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {width, height, 1};

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	EndSingleTimeCommands(commandBuffer);
}

void RenderSystem::CreateTextureMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageMemoryBarrier &barrier)
{
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	}
	else
	{
		std::cout << "[ERROR] Unsupported image layout transition!" << std::endl;
	}
}

void RenderSystem::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;	  // TODO
	barrier.dstAccessMask = 0;	  // TODO

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("[ERROR] Unsupported layout transition!");
	}

	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	EndSingleTimeCommands(commandBuffer);
}

VkCommandBuffer RenderSystem::BeginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void RenderSystem::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void RenderSystem::GenerateMissingTexture(uint32 width, uint32 tiles, void* textureMemory)
{
	uint32* img = reinterpret_cast<uint32*>(textureMemory);
	uint32 stride = width / tiles;

	for (uint32 y = 0; y < width; y++)
	{
		for (uint32 x = 0; x < width; x++)
		{
			uint32 xTile = x / stride;
			uint32 yTile = y / stride;
			img[x + width * y] = (xTile % 2) ^ (yTile % 2) ? 0xFFFF00FF : 0xFF000000;
		}
	}
}

std::vector<char> RenderSystem::LoadSPIRV(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("[ERROR] Failed to open shader file!");
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

VkShaderModule RenderSystem::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo{};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create shader module!");
	}

	return shaderModule;
}

void RenderSystem::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChainExtent;
	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	renderPassInfo.clearValueCount = static_cast<uint32>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	for (size_t i = 0; i < drawStaticCommands.size(); i++)
	{
		SPushConstants pushConstant = {camera, drawStaticCommands[i].modelTransform};	
		vkCmdPushConstants(
			commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SPushConstants), &pushConstant);
		
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
			&materials[drawStaticCommands[i].material].descriptor, 0, nullptr);
		
		VkBuffer vertexBuffer = deviceMemory[meshes[drawStaticCommands[i].mesh].vertexBlock].buffer;
		VkDeviceSize vertexOffset = meshes[drawStaticCommands[i].mesh].vertexOffset;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &vertexOffset);
		
		vkCmdBindIndexBuffer(commandBuffer, deviceMemory[meshes[drawStaticCommands[i].mesh].indexBlock].buffer,
			meshes[drawStaticCommands[i].mesh].indexOffset, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(commandBuffer, meshes[drawStaticCommands[i].mesh].indices, 1, 0, 0, 0);
	}

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to record command buffer!");
	}
}

void RenderSystem::RecreateSwapChain()
{
	vkDeviceWaitIdle(device);
	DestroySwapChain();
	InitSwapChain();
	InitDepthbuffer();
	InitSwapChainFramebuffers();
}