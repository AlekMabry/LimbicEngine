#include "Renderer/RView.h"
#include "System/RenderSystem.h"
#include "Renderer/RWindow.h"

#include <glm/ext/matrix_clip_space.hpp>


RView::RView(RenderSystem* pRenderSystem, RWindow* pRenderWindow, std::function<void()> frameReady)
	: pR(pRenderSystem), pW(pRenderWindow), frameReadyFunc(std::move(frameReady))
{
}

RView::~RView()
{
}

void RView::SetViewTransform(const mat4& viewTransform)
{
	cameraView = viewTransform;
}

void RView::SetProjection(float zNear, float zFar, float fovY)
{
	cameraNear = zNear;
	cameraFar = zFar;
	cameraFov = fovY;
	cameraOrthographic = (fovY == 0.0f);
	const auto &imgSize = pW->GetSwapChainImageSize();
	const auto aspect = static_cast<float>(imgSize.first) / static_cast<float>(imgSize.second);
	cameraProj = glm::perspective(glm::radians(fovY), aspect, zNear, zFar);
	cameraProj[1][1] *= -1;
}

void RView::StartNextFrame()
{
	//vkResetCommandBuffer(pW->GetCurrentCommandBuffer(), 0);
	RecordCommandBuffer(pW->GetCurrentCommandBuffer());

	frameReadyFunc();
}

void RView::InitRenderPass()
{
	/*
	 color0, mask, depth => [Subpass 0: Forward PBR] =>
	 color0, mask (as texture), color1 => [Subpass 1: Postprocess] =>
	 present color1 
	 */

	// Forward-render color
	VkAttachmentDescription subpass0ColorAttachment{};
	subpass0ColorAttachment.format = pW->GetColorFormat();
	subpass0ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	subpass0ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	subpass0ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	subpass0ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	subpass0ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	subpass0ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	subpass0ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	VkAttachmentReference subpass0ColorOutputRef = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
	VkAttachmentReference subpass1ColorInputRef = {0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

	// Editor selected object mask and object IDs
	VkAttachmentDescription subpass0MaskAttachment{};
	subpass0MaskAttachment.format = pW->GetColorFormat();
	subpass0MaskAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	subpass0MaskAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	subpass0MaskAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	subpass0MaskAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	subpass0MaskAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	subpass0MaskAttachment.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	subpass0MaskAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	VkAttachmentReference subpass0MaskOutputRef = {1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

	// Forward-render depth
	VkAttachmentDescription subpass0DepthAttachment{};
	subpass0DepthAttachment.format = pW->GetDepthStencilFormat();
	subpass0DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	subpass0DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	subpass0DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	subpass0DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	subpass0DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	subpass0DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	subpass0DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	VkAttachmentReference depth0OutputRef = {2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

	// Postprocess output to swap chain
	VkAttachmentDescription subpass1PresentAttachment{};
	subpass1PresentAttachment.format = pW->GetColorFormat();
	subpass1PresentAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	subpass1PresentAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	subpass1PresentAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	subpass1PresentAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	subpass1PresentAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	subpass1PresentAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	subpass1PresentAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkAttachmentReference subpass1PresentOutputRef = {3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

	// Subpass 0
	std::array colorRefs0 = {subpass0ColorOutputRef, subpass0MaskOutputRef};
	VkSubpassDescription subpass0{};
	subpass0.colorAttachmentCount = static_cast<uint32_t>(colorRefs0.size());
	subpass0.pColorAttachments = colorRefs0.data();
	subpass0.pDepthStencilAttachment = &depth0OutputRef;

	// Subpass 1 (postprocess)
	VkSubpassDescription subpass1{};
	subpass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	std::array inputRefs1 = {subpass1ColorInputRef};
	subpass1.inputAttachmentCount = static_cast<uint32_t>(inputRefs1.size());
	subpass1.pInputAttachments = inputRefs1.data();
	subpass1.colorAttachmentCount = 1;
	subpass1.pColorAttachments = &subpass1PresentOutputRef;
	subpass1.pDepthStencilAttachment = nullptr;

	VkSubpassDependency externalTo0{};
	externalTo0.srcSubpass = VK_SUBPASS_EXTERNAL;
	externalTo0.dstSubpass = 0;
	externalTo0.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	externalTo0.srcAccessMask = 0;
	externalTo0.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	externalTo0.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency dependency0to1{};
	dependency0to1.srcSubpass = 0;
	dependency0to1.dstSubpass = 1;
	dependency0to1.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency0to1.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency0to1.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependency0to1.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT;

	std::array attachments =
		{subpass0ColorAttachment, subpass0MaskAttachment, subpass0DepthAttachment, subpass1PresentAttachment};
	std::array subpasses = {subpass0, subpass1};
	std::array dependencies = {externalTo0, dependency0to1};

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
	renderPassInfo.pSubpasses = subpasses.data();
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VK_CHECK(vkCreateRenderPass(pR->device, &renderPassInfo, nullptr, &renderPass));
}

void RView::DestroyRenderPass()
{
	vkDestroyRenderPass(pR->device, renderPass, nullptr);
}

void RView::InitGraphicsPipeline()
{
	// Static PBR is a forward pass that targets a color image and model index image
	auto vShdCode_staticPBR = pR->LoadSPIRV("C:/Users/alekm/Desktop/LimbicEngine/build/Debug/Shader/staticPBRVert.spv");
	VkShaderModule vShdModule_staticPBR = pR->CreateShaderModule(vShdCode_staticPBR);
	auto fShdCode_staticPBR = pR->LoadSPIRV("C:/Users/alekm/Desktop/LimbicEngine/build/Debug/Shader/staticPBRFrag.spv");
	VkShaderModule fShdModule_staticPBR = pR->CreateShaderModule(fShdCode_staticPBR);

	VkPipelineShaderStageCreateInfo vShdStageInfo_staticPBR{};
	vShdStageInfo_staticPBR.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vShdStageInfo_staticPBR.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vShdStageInfo_staticPBR.module = vShdModule_staticPBR;
	vShdStageInfo_staticPBR.pName = "main";
	// vertShaderStageInfo.pSpecializationInfo - Important in future

	VkPipelineShaderStageCreateInfo fShdStageInfo_staticPBR{};
	fShdStageInfo_staticPBR.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fShdStageInfo_staticPBR.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fShdStageInfo_staticPBR.module = fShdModule_staticPBR;
	fShdStageInfo_staticPBR.pName = "main";
	// fragShaderStageInfo.pSpecializationInfo - Important in future

	VkPipelineShaderStageCreateInfo shaderStages[] = {vShdStageInfo_staticPBR, fShdStageInfo_staticPBR};

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
	viewport.width = (float) pW->GetSwapChainExtent().width;
	viewport.height = (float) pW->GetSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = pW->GetSwapChainExtent();

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

	VkPipelineColorBlendAttachmentState subpass0_colorBlendAttachment{};
	subpass0_colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	subpass0_colorBlendAttachment.blendEnable = VK_TRUE;
	subpass0_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	subpass0_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	subpass0_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	subpass0_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	subpass0_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	subpass0_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendAttachmentState subpass0_maskBlendAttachment{};
	subpass0_maskBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	subpass0_maskBlendAttachment.blendEnable = VK_TRUE;
	subpass0_maskBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	subpass0_maskBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	subpass0_maskBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	subpass0_maskBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	subpass0_maskBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	subpass0_maskBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	std::array subpass0_blendAttachments = {subpass0_colorBlendAttachment, subpass0_maskBlendAttachment};

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = static_cast<uint32>(subpass0_blendAttachments.size());
	colorBlending.pAttachments = subpass0_blendAttachments.data();
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SPushConstants);
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_PBR;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	VK_CHECK(vkCreatePipelineLayout(pR->device, &pipelineLayoutInfo, nullptr, &pipelineLayout_graphics));

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
	pipelineInfo.layout = pipelineLayout_graphics;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;	// Used to derive new pipeline from existing

	VK_CHECK(vkCreateGraphicsPipelines(pR->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_graphics));

	vkDestroyShaderModule(pR->device, fShdModule_staticPBR, nullptr);
	vkDestroyShaderModule(pR->device, vShdModule_staticPBR, nullptr);
}

void RView::InitPostprocessPipeline()
{
	// Composite uses a color image and model index image (quad model) to draw outlines on a selected model
	auto vShdCode_postprocess = pR->LoadSPIRV("C:/Users/alekm/Desktop/LimbicEngine/build/Debug/Shader/postprocessVert.spv");
	VkShaderModule vShdModule_postprocess = pR->CreateShaderModule(vShdCode_postprocess);
	auto fShdCode_postprocess = pR->LoadSPIRV("C:/Users/alekm/Desktop/LimbicEngine/build/Debug/Shader/postprocessFrag.spv");
	VkShaderModule fShdModule_postprocess = pR->CreateShaderModule(fShdCode_postprocess);

	VkPipelineShaderStageCreateInfo vShdStageInfo_postprocess{};
	vShdStageInfo_postprocess.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vShdStageInfo_postprocess.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vShdStageInfo_postprocess.module = vShdModule_postprocess;
	vShdStageInfo_postprocess.pName = "main";
	// vertShaderStageInfo.pSpecializationInfo - Important in future

	VkPipelineShaderStageCreateInfo fShdStageInfo_postprocess{};
	fShdStageInfo_postprocess.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fShdStageInfo_postprocess.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fShdStageInfo_postprocess.module = fShdModule_postprocess;
	fShdStageInfo_postprocess.pName = "main";
	// fragShaderStageInfo.pSpecializationInfo - Important in future

	VkPipelineShaderStageCreateInfo shaderStages[] = {vShdStageInfo_postprocess, fShdStageInfo_postprocess};

	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;	   // Optional
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;	 // Optional

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) pW->GetSwapChainExtent().width;
	viewport.height = (float) pW->GetSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = pW->GetSwapChainExtent();

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
	rasterizerCreateInfo.cullMode = VK_CULL_MODE_NONE;
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

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

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

	// VkPushConstantRange pushConstantRange{};
	// pushConstantRange.offset = 0;
	// pushConstantRange.size = sizeof(SPushConstants);
	// pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_postprocess;
	// pipelineLayoutInfo.pushConstantRangeCount = 1;
	// pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	VK_CHECK(vkCreatePipelineLayout(pR->device, &pipelineLayoutInfo, nullptr, &pipelineLayout_postprocess));

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputCreateInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
	pipelineInfo.layout = pipelineLayout_postprocess;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;	// Used to derive new pipeline from existing

	VK_CHECK(vkCreateGraphicsPipelines(pR->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_postprocess));

	vkDestroyShaderModule(pR->device, fShdModule_postprocess, nullptr);
	vkDestroyShaderModule(pR->device, vShdModule_postprocess, nullptr);
}

void RView::InitGraphicsCommandPool()
{
	if (pR->commandPool)
		return;

	SQueueFamilyIndices queueFamilyIndices = pR->FindQueueFamilies(pR->physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(pR->device, &poolInfo, nullptr, &pR->commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] Failed to create command pool!");
	}
}

void RView::InitDescriptorPool()
{
	VkDescriptorPoolSize poolSizes_PBR[4];
	poolSizes_PBR[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes_PBR[0].descriptorCount = 16;
	poolSizes_PBR[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes_PBR[1].descriptorCount = 16;
	poolSizes_PBR[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes_PBR[2].descriptorCount = 16;
	poolSizes_PBR[3].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	poolSizes_PBR[3].descriptorCount = 16;

	VkDescriptorPoolCreateInfo poolInfo_PBR{};
	poolInfo_PBR.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo_PBR.poolSizeCount = 4;
	poolInfo_PBR.pPoolSizes = poolSizes_PBR;
	poolInfo_PBR.maxSets = 16;

	VK_CHECK(vkCreateDescriptorPool(pR->device, &poolInfo_PBR, nullptr, &descriptorPool_PBR));
}

void RView::InitDescriptorSetLayout()
{
	std::array<VkDescriptorSetLayoutBinding, 3> bindings_PBR = {};

	bindings_PBR[0].binding = 0;
	bindings_PBR[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings_PBR[0].descriptorCount = 1;
	bindings_PBR[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings_PBR[0].pImmutableSamplers = nullptr;

	bindings_PBR[1].binding = 1;
	bindings_PBR[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings_PBR[1].descriptorCount = 1;
	bindings_PBR[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings_PBR[1].pImmutableSamplers = nullptr;

	bindings_PBR[2].binding = 2;
	bindings_PBR[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings_PBR[2].descriptorCount = 1;
	bindings_PBR[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings_PBR[2].pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo_PBR{};
	layoutInfo_PBR.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo_PBR.bindingCount = static_cast<uint32>(bindings_PBR.size());
	layoutInfo_PBR.pBindings = bindings_PBR.data();

	VK_CHECK(vkCreateDescriptorSetLayout(pR->device, &layoutInfo_PBR, nullptr, &descriptorSetLayout_PBR));

	// Postprocess

	std::array<VkDescriptorSetLayoutBinding, 2> bindings_postprocess = {};

	bindings_postprocess[0].binding = 3;
	bindings_postprocess[0].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	bindings_postprocess[0].descriptorCount = 1;
	bindings_postprocess[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings_postprocess[0].pImmutableSamplers = nullptr;

	bindings_postprocess[1].binding = 1;
	bindings_postprocess[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings_postprocess[1].descriptorCount = 1;
	bindings_postprocess[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings_postprocess[1].pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo_postprocess{};
	layoutInfo_postprocess.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo_postprocess.bindingCount = static_cast<uint32>(bindings_postprocess.size());
	layoutInfo_postprocess.pBindings = bindings_postprocess.data();

	VK_CHECK(vkCreateDescriptorSetLayout(pR->device, &layoutInfo_postprocess, nullptr, &descriptorSetLayout_postprocess));
}

void RView::BindMaskDescriptorSet()
{
	std::array layouts = {descriptorSetLayout_postprocess, descriptorSetLayout_postprocess,
		descriptorSetLayout_postprocess};
	descriptorSet_postprocess.resize(3);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool_PBR;
	allocInfo.descriptorSetCount = 3;
	allocInfo.pSetLayouts = layouts.data();

	VK_CHECK(vkAllocateDescriptorSets(pR->device, &allocInfo, descriptorSet_postprocess.data()));

	for (int i = 0; i < 3; i++)
	{
		VkDescriptorImageInfo maskInfo{};
		maskInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		maskInfo.imageView = pW->subpass0_maskImageViews[i];
		maskInfo.sampler = textureSampler;

		VkDescriptorImageInfo inputAttachmentInfo{};
		inputAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		inputAttachmentInfo.imageView = pW->subpass0_colorImageView;
		inputAttachmentInfo.sampler = VK_NULL_HANDLE;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSet_postprocess[i];
		descriptorWrites[0].dstBinding = 1;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pImageInfo = &maskInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSet_postprocess[i];
		descriptorWrites[1].dstBinding = 3;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &inputAttachmentInfo;

		vkUpdateDescriptorSets(pR->device, static_cast<uint32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void RView::InitDefaultTextureSampler()
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

	VK_CHECK(vkCreateSampler(pR->device, &samplerInfo, nullptr, &textureSampler));
}

void RView::RecordCommandBuffer(VkCommandBuffer commandBuffer)
{
	/*
	 *	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));*/

	std::array<VkClearValue, 4> clearValues{};
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};	// Subpass 0 color
	clearValues[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};	// Subpass 0 mask
	clearValues[2].depthStencil = {1.0f, 0};			// Subpass 0 depth
	clearValues[3].color = {{0.0f, 0.0f, 0.0f, 1.0f}};	// Subpass 1 color

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = pW->GetCurrentFramebuffer();
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = pW->GetSwapChainExtent();
	renderPassInfo.clearValueCount = static_cast<uint32>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	barrier.image = pW->subpass0_maskImages[pW->GetCurrentSwapChainImageIndex()];
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
		0, nullptr, 1, &barrier);

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_graphics);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(renderPassInfo.renderArea.extent.width);
	viewport.height = static_cast<float>(renderPassInfo.renderArea.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = renderPassInfo.renderArea.extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	for (const auto &cmd : pR->drawStaticCommands)
	{
		SPushConstants pushConstant = {
			GetCameraMat(), cmd.modelTransform, cmd.selected};
		vkCmdPushConstants(commandBuffer, pipelineLayout_graphics, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
			sizeof(SPushConstants), &pushConstant);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_graphics, 0, 1,
			&pR->materials[cmd.material].descriptor, 0, nullptr);

		VkBuffer vertexBuffer = pR->deviceMemory[pR->meshes[cmd.mesh].vertexBlock].buffer;
		VkDeviceSize vertexOffset = pR->meshes[cmd.mesh].vertexOffset;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &vertexOffset);

		vkCmdBindIndexBuffer(commandBuffer, pR->deviceMemory[pR->meshes[cmd.mesh].indexBlock].buffer,
			pR->meshes[cmd.mesh].indexOffset, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(commandBuffer, pR->meshes[cmd.mesh].indices, 1, 0, 0, 0);
	}

	// Postprocess quad
	vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_postprocess);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_postprocess, 0, 1,
		&descriptorSet_postprocess[pW->GetCurrentSwapChainImageIndex()], 0, nullptr);
	vkCmdDraw(commandBuffer, 6, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	//VK_CHECK(vkEndCommandBuffer(commandBuffer));
}

mat4 RView::GetCameraMat()
{
	return cameraProj * cameraView;
}
