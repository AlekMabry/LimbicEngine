#pragma once

#include <Renderer/RView.h>

#include <QVulkanWindowRenderer>

class Game;

class RView_Qt : public RView, public QVulkanWindowRenderer
{
public:
	RView_Qt(Game* pGame, RenderSystem* pRenderSystem, RWindow_Qt* pWindow);

	~RView_Qt();

	/**** QVulkanWindowRenderer ****/

	void initResources() override;

	void initSwapChainResources() override;

	void releaseSwapChainResources() override;

	// void releaseResources() override;

	void startNextFrame() override;

private:
	Game *pG;
};
