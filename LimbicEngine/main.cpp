#include "io.hpp"
#include "common.hpp"
#include "player.hpp"
#include "map.hpp"
#include "render.hpp"
#include "pc_gl_render.hpp"

int main() {
	io statusStream;
	pc_gl_render renderEngine(&statusStream, 1280, 720, 90);
	map cMap(&renderEngine);

	GLFWwindow* window = renderEngine.getWindow();
	player mainPlayer(window, &renderEngine);

	// Load the map and print any debugging data
	cMap.load("outpost731/maps/731_c1a0.bsp", "outpost731/textures/halflife.wad");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do {
		
		mainPlayer.update();
		mainPlayer.pushPerspective();
		cMap.compileBuffers();
		cMap.pushBuffers();
		renderEngine.render3D();
		renderEngine.uploadFramebuffer();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
