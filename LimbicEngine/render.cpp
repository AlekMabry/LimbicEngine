#include "render.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "shaderLoader.hpp"
#include "textureLoader.hpp"

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

void render::usePlayerPerspective()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 mainPlayerPos = mainPlayer->getPosition();
	vec3 mainPlayerDir = mainPlayer->getDirection();
	gl_transform_projection = perspective(radians(FOV), (float) screenWidth / (float) screenHeight, (float) 0.1, (float) 2048.0);
	gl_transform_view = lookAt(
		mainPlayerPos,					// Camera position in world space
		mainPlayerPos + mainPlayerDir,
		vec3(0, 0, 1)					//Head is up
	);
	gl_transform_world_view = gl_transform_projection * gl_transform_view;
}

void render::drawTris(int shader, int size, GLuint vertexData, GLuint uvData, float width, float height)
{
	/*
	SETUP SHADER
	Shader ID's are as follows:
	0 - Full White
	1 - Solid Texture
	2 - Solid Texture Lightmap
	3 - Transparent Texture
	*/

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	//Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glUseProgram(mapShader_default);

	GLuint MatrixID = glGetUniformLocation(mapShader_default, "MATRIX_WORLD_VIEW_PROJECTION");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &gl_transform_world_view[0][0]);

	GLuint textureDimensions = glGetUniformLocation(mapShader_default, "in_texDimensions");
	glUniform2f(textureDimensions, width, height);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, size); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

void render::uploadFramebuffer()
{
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
