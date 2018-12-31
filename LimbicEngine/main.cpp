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
#include "hud.hpp"

int main() {
	GLFWwindow* window;
	map cMap;
	render renderEngine(1280, 720, 110);
	hud HUD;

	// Exchange object handles
	window = renderEngine.getWindowPointer();
	player mainPlayer(window);
	renderEngine.setPlayerPointer(&mainPlayer);
	cMap.setRenderHandle(&renderEngine);

	// Load the map and print any debugging data
	cMap.load("outpost731/maps/731_c1a0.bsp", "outpost731/textures/halflife.wad");
	HUD.loadFont();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do {
		
		mainPlayer.update();
		renderEngine.usePlayerPerspective();
		cMap.renderMap();
		HUD.renderText("OpenGL", 64, 64, 1, glm::vec3(0.0, 1.0, 0.0));
		renderEngine.uploadFramebuffer();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
