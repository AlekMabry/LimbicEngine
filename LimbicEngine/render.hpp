#ifndef RENDER_HPP
#define RENDER_HPP

#include "common.hpp"
#include "player.hpp"

class render
{
private:
	GLFWwindow* window;
	player* mainPlayer;

	mat4 gl_transform_projection;
	mat4 gl_transform_view;
	mat4 gl_transform_world_view;

public:
	// Temporary/Ect
	GLuint tex_missing;

	// Shaders
	GLuint mapShader_default;
	GLuint lightmap01;

	// Settings
	int screenWidth;
	int screenHeight;
	float FOV;

	render(int width, int height, float fov);
	void usePlayerPerspective();
	void drawTris(int shader, int size, GLuint vertexData, GLuint uvData, float width, float height);
	int draw_2D(int shader, int size, GLuint vertexData, GLuint uvData, float width);
	void uploadFramebuffer();
	
	// Getters and Setters
	GLFWwindow* getWindowPointer();
	void setPlayerPointer(player* playerObject);
};

#endif