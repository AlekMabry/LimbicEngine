#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "common.hpp"

#include "shaderLoader.hpp"
#include "textureLoader.hpp"
#include "player.hpp"
#include "bspHandler.hpp"
#include "render.hpp"
// #include "vtfHandler.hpp"

int main() {
	// vtfHandler vtfObject;
	// vtfObject.loadVTF("outpost731/textures/concrete.vtf");
	// vtfObject.printInfo();

	GLFWwindow* window;
	bspHandler bspObject;
	render renderEngine(1024, 768, 90);

	// Exchange object pointers
	window = renderEngine.getWindowPointer();
	player mainPlayer(window);
	renderEngine.setPlayerPointer(&mainPlayer);
	bspObject.setRenderPointer(&renderEngine);

	// Load the map and print any debugging data
	bspObject.loadBSP("outpost731/maps/731_c1a0.bsp", true);
	bspObject.compileBuffers();
	bspObject.printLump();
	bspObject.printTexPointers();
	bspObject.printTexInfo();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do {
		
		mainPlayer.update();
		renderEngine.usePlayerPerspective();
		bspObject.renderMap();
		renderEngine.uploadFramebuffer();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
