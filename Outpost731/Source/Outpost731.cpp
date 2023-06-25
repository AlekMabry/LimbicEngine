#include <Game.h>
#include <System/IOSystem_GLFW.h>

int main()
{
	Game game("Outpost 731");
	IOSystem_GLFW ioSystem(game.GetApplicationName().c_str(), 1280, 720);
	game.SetIO(ioSystem);
	game.Run();

	return EXIT_SUCCESS;
}