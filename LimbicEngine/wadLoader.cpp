#include "wadLoader.hpp"

int wadHandler::openWad(const char *fname)
{

	if (fopen_s(&wadFile, fname, "rb"))
	{
		fprintf(stderr, "[error] BSP failed to open!\n");
		return 1;
	}

	// Load the header into memory so we can analyze it
	// fread(&header, sizeof(header), 1, wadFile);

	return 1;
}