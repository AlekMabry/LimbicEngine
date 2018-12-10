#include "vtfHandler.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>

int vtfHandler::loadVTF(const char *fname)
{
	FILE* vtfFile;

	if (fopen_s(&vtfFile, fname, "rb"))
	{
		fprintf(stderr, "[error] BSP failed to open!\n");
		return 1;
	}

	// Load the header into memory so we can analyze it
	fread(&header, sizeof(header), 1, vtfFile);

	return 1;
}

void vtfHandler::printInfo()
{
	std::cout << "[VTF] File Signature: ";
	std::cout << header.signature << "\n";
}