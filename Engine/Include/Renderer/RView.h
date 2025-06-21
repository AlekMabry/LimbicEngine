#pragma once

#include "LimbicTypes.h"

#include <vector>
#include <vulkan/vulkan.h>

#include <functional>

class RenderSystem;
class RWindow;

class RView
{
	friend class RenderSystem;
	friend class RWindow;
	friend class RWindow_GLFW;

public:
	RView(RenderSystem *pRenderSystem, RWindow *pRenderWindow, std::function<void()> frameReadyFunc);

	~RView();

	/**** Game/EEntity interface. ****/

	/** Sets camera view transform (ie. lookAt matrix) */
	void SetViewTransform(const mat4 &viewTransform);

	/** Configures camera projection matrix, fovY is in degrees, fovY = 0.0f sets orthographic. */
	void SetProjection(float zNear, float zFar, float fovY);

	void InitGraphicsPipeline();
	void InitGraphicsCommandPool();
	void InitDescriptorPool();
	void InitDescriptorSetLayout();
	void InitDefaultTextureSampler();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkExtent2D extent);

protected:
	RenderSystem *pR;
	RWindow *pW;

	mat4 cameraView;
	bool cameraOrthographic;
	float cameraFov;
	float cameraNear;
	float cameraFar;
	mat4 cameraProj;

	std::function<void()> frameReadyFunc;	// Called when commands are done queueing.

	VkSampler textureSampler;

	// Pipelines
	VkDescriptorPool descriptorPool_PBR;
	VkDescriptorSetLayout descriptorSetLayout_PBR;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

private:
	mat4 GetCameraMat();
};