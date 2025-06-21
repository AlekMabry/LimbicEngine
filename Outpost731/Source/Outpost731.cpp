#include "Game.h"
#include "System/RenderSystem.h"
#include "Renderer/RWindow.h"

int main()
{
	Game game("Outpost 731");
	game.CreateWin("main", 1280, 720);
	game.Run();

	return EXIT_SUCCESS;
}