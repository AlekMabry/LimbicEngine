#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

#include "shaderLoader.hpp"
#include "textureLoader.hpp"
#include "player.hpp"
#include "bspHandler.hpp"
#include "render.hpp"

render::render(int width, int height, float fov)
{
	screenWidth = width;
	screenHeight = height;
	FOV = fov;

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	window = glfwCreateWindow(screenWidth, screenHeight, "Outpost 731", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	//Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Load shaders and compile
	mapShader_default = LoadShaders("outpost731/shaders/map_default_vert.shader", "outpost731/shaders/map_default_frag.shader");

	// Load textures
	tex_missing = loadBMP_custom("outpost731/textures/missingTexture.bmp");
}

void render::bsp_loadBuffers()
{
	bsp->compileTris(map_triData, map_uvData, map_lightmap_uvData);

	// Create the VAOs (Vertex Array Objects)
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create buffers (VBO)
	glGenBuffers(1, &map_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, map_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, map_triData.size() * sizeof(GLfloat), &map_triData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &map_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, map_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, map_uvData.size() * sizeof(GLfloat), &map_uvData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &map_lightmap_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, map_lightmap_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, map_lightmap_uvData.size() * sizeof(GLfloat), &map_lightmap_uvData[0], GL_STATIC_DRAW);
}

void render::renderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 mainPlayerPos = mainPlayer->getPosition();
	vec3 mainPlayerDir = mainPlayer->getDirection();
	mat4 Projection = perspective(radians(FOV), (float) screenWidth / (float) screenHeight, (float) 0.1, (float) 2048.0);
	mat4 View = lookAt(
		mainPlayerPos,					// Camera position in world space
		mainPlayerPos+mainPlayerDir,
		vec3(0, 0, 1)					//Head is up
	);

	mat4 MATRIX_WORLD_VIEW_PROJECTION = Projection*View;

	glUseProgram(mapShader_default);

	GLuint MatrixID = glGetUniformLocation(mapShader_default, "MATRIX_WORLD_VIEW_PROJECTION");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MATRIX_WORLD_VIEW_PROJECTION[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, map_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, map_uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, map_lightmap_uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, map_triData.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWwindow * render::getWindowPointer()
{
	return window;
}

void render::setPlayerPointer(player * playerObject)
{
	mainPlayer = playerObject;
}

void render::setBspPointer(bspHandler* bspHandlerObject)
{
	bsp = bspHandlerObject;
}
