#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "common.hpp"

#include "shaderLoader.hpp"
#include "textureLoader.hpp"
#include "player.hpp"
#include "map.hpp"
#include "render.hpp"

int main() {
	GLFWwindow* window;
	map cMap;
	render renderEngine(1024, 768, 90);

	// Exchange object handles
	window = renderEngine.getWindowPointer();
	player mainPlayer(window);
	renderEngine.setPlayerPointer(&mainPlayer);
	cMap.setRenderHandle(&renderEngine);

	// Load the map and print any debugging data
	cMap.load("outpost731/maps/731_c1a0.bsp", "outpost731/textures/halflife.wad");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do {
		
		mainPlayer.update();
		renderEngine.usePlayerPerspective();
		cMap.renderMap();
		renderEngine.uploadFramebuffer();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
