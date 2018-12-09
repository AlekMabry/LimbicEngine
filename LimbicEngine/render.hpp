#ifndef RENDER_HPP
#define RENDER_HPP

class render
{
private:
	GLFWwindow* window;
	player* mainPlayer;

public:
	// Pointers to other objects
	bspHandler* bsp;

	// Raw data
	std::vector<GLfloat> map_triData;
	std::vector<GLfloat> map_uvData;
	std::vector<GLfloat> map_lightmap_uvData;

	// VAO's
	GLuint VertexArrayID;

	// VBO's
	GLuint map_vertexBuffer;
	GLuint map_uvBuffer;
	GLuint map_lightmap_uvBuffer;

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
	void bsp_loadBuffers();
	void renderFrame();
	
	// Getters and Setters
	GLFWwindow* getWindowPointer();
	void setPlayerPointer(player* playerObject);
	void setBspPointer(bspHandler* bspHandlerObject);
};

#endif