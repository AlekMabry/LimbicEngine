#include <Game.h>

int main()
{
	std::unique_ptr<Game> game;

	try {
		game = std::make_unique<Game>();
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	try {
		game->Run();
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}