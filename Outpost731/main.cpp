#include <Game.h>

int main()
{
	//std::string fbx = "C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx";
	//std::string object = "Truss";
	//resources->LoadMesh(fbx, object);

	auto game = std::make_unique<Game>();

	try {
		game->Run();
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}