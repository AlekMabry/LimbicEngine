#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
using namespace glm;

#include "shaderLoader.hpp"
#include "textureLoader.hpp"
#include "player.hpp"
#include "bspHandler.hpp"
#include "render.hpp"

int main() {
	GLFWwindow* window;
	bspHandler bspObject;
	render renderEngine(1024, 768, 90);

	// Exchange object pointers
	window = renderEngine.getWindowPointer();
	player mainPlayer(window);
	renderEngine.setBspPointer(&bspObject);
	renderEngine.setPlayerPointer(&mainPlayer);

	// Load the map and print any debugging data
	bspObject.loadBSP("outpost731/maps/boxmap_v2.bsp");
	renderEngine.bsp_loadBuffers();
	bspObject.printLump();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do {
		
		mainPlayer.update();
		renderEngine.renderFrame();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
