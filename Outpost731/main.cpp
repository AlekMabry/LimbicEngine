#include <Game.h>
#include <GLFW/glfw3.h>
#include <memory>

uint32 windowWidth = 1280;
uint32 windowHeight = 720;

/** Called when GLFW window is resized. */
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

int main()
{
	

	//std::string fbx = "C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx";
	//std::string object = "Truss";
	//resources->LoadMesh(fbx, object);

	/** Initialize GLFW Window */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Outpost 731", NULL, NULL);
	if (!window)
	{
		std::cerr << "[ERROR] Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[ERROR] Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	/** Initialize game */
	std::unique_ptr<Game> game = std::make_unique<Game>();
	ResourceManager* resources = game->GetResourceManager();

	while (!glfwWindowShouldClose(window))
	{
		game->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}