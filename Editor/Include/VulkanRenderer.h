#pragma once

#include "Renderer/RView.h"

#include <QVulkanWindowRenderer>

class Game;

class VulkanRenderer : public QVulkanWindowRenderer, public RView
{
public:
	VulkanRenderer(Game* pGame, RenderSystem* pRenderSystem, QVulkanWindow* pWindow);

	~VulkanRenderer();

	/**** QVulkanWindowRenderer ****/

	void initResources() override;

	// void initSwapChainResources() override;

	// void releaseSwapChainResources() override;

	// void releaseResources() override;

	void startNextFrame() override;

private:
	Game *pG;
};
