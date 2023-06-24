#include <Game.h>
#include <System/IOSystem_GLFW.h>

int main()
{
	Game game(std::string("Outpost 731"));
	IOSystem_GLFW ioSystem("Outpost 731", 1280, 720);
	game.SetIO(static_cast<IOSystem*>(&ioSystem));
	game.Run();

	return EXIT_SUCCESS;
}