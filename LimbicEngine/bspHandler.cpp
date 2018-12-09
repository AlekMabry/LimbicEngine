#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "bspHandler.hpp"

int bspHandler::loadBSP(const char *fname) 
{
	FILE* mapFile;

	if (fopen_s(&mapFile, fname, "rb"))
	{
		fprintf(stderr, "[error] BSP failed to open!\n");
		return 1;
	}

	// Load the header into memory so we can analyze it
	fread(&map_header, sizeof(map_header), 1, mapFile);

	int arrayLength;

	// Load vertexes
	arrayLength = (int)((float)map_header.lumps[3].filelen / (float) sizeof(Vector));
	vertexArray = new Vector[arrayLength];
	vertexArrayLength = arrayLength;
	fseek(mapFile, map_header.lumps[3].fileofs, SEEK_SET);
	for (int i = 0; i < vertexArrayLength; i++) {
		fread(&vertexArray[i], sizeof(Vector), 1, mapFile);
	}

	// Load texinfo
	arrayLength = (int)((float)map_header.lumps[6].filelen / (float) sizeof(dtexinfo_t));
	texinfoArray = new dtexinfo_t[arrayLength];
	texinfoArrayLength = arrayLength;
	fseek(mapFile, map_header.lumps[6].fileofs, SEEK_SET);
	for (int i = 0; i < texinfoArrayLength; i++) {
		fread(&texinfoArray[i], sizeof(dtexinfo_t), 1, mapFile);
	}

	// Load edges
	arrayLength = (int)((float)map_header.lumps[12].filelen / (float) sizeof(dedge_t));
	edgeArray = (dedge_t*)calloc(arrayLength, sizeof(dedge_t)); // new dedge_t[arrayLength];
	edgeArrayLength = arrayLength;
	fseek(mapFile, map_header.lumps[12].fileofs, SEEK_SET);
	for (int i = 0; i < edgeArrayLength; i++) {
		fread(&edgeArray[i], sizeof(dedge_t), 1, mapFile);
	}

	// Load surfedges
	arrayLength = (int)((float)map_header.lumps[13].filelen / (float) sizeof(dsurfedge_t));
	surfedgeArray = new dsurfedge_t[arrayLength];
	surfedgeArrayLength = arrayLength;
	fseek(mapFile, map_header.lumps[13].fileofs, SEEK_SET);
	for (int i = 0; i < surfedgeArrayLength; i++) {
		fread(&surfedgeArray[i], sizeof(dsurfedge_t), 1, mapFile);
	}

	// Load face
	arrayLength = (int)((float)map_header.lumps[7].filelen / (float) sizeof(dface_t));
	faceArray = new dface_t[arrayLength];
	faceArrayLength = arrayLength;
	fseek(mapFile, map_header.lumps[7].fileofs, SEEK_SET);
	for (int i = 0; i < faceArrayLength; i++) {
		fread(&faceArray[i], sizeof(dface_t), 1, mapFile);
	}

	// Load lightmap pixels
	arrayLength = (int)((float)map_header.lumps[8].filelen / (float) sizeof(ColorRGBExp32));
	lightmapPixelArray = new ColorRGBExp32[arrayLength];
	lightmapPixelArrayLength = arrayLength;
	fseek(mapFile, map_header.lumps[8].fileofs, SEEK_SET);
	for (int i = 0; i < lightmapPixelArrayLength; i++) {
		fread(&lightmapPixelArray[i], sizeof(ColorRGBExp32), 1, mapFile);
	}

	if (mapFile)
	{
		if (fclose(mapFile))
		{
			fprintf(stderr, "[error] BSP failed to close!\n");
		}
	}
	
	return 0;
}

void bspHandler::compileTris(std::vector<GLfloat> &triData, std::vector<GLfloat> &uvData, std::vector<GLfloat> &lightmap_uvData)
{
	int edgeIndex, v;
	Vector vert0;
	Vector vert1;
	Vector vert2;

	float uv0[2];
	float uv1[2];
	float uv2[2];

	float lightuv0[2];
	float lightuv1[2];
	float lightuv2[2];

	for (int i = 0; i < map_header.lumps[7].filelen / sizeof(dface_t); i++) {
		short texInfoIndex = faceArray[i].texinfo;

		signed int edgeID = surfedgeArray[faceArray[i].firstedge].edgeID;
		if (edgeID > 0) {
			// Get first vertex
			vert0 = vertexArray[edgeArray[abs(edgeID)].v[0]];
		}
		else 
		{
			// Get second vertex
			vert0 = vertexArray[edgeArray[abs(edgeID)].v[1]];
		}

		uv0[0] = (texinfoArray[texInfoIndex].textureVecs[0][0] * vert0.x) + (texinfoArray[texInfoIndex].textureVecs[0][1] * vert0.y) +
				 (texinfoArray[texInfoIndex].textureVecs[0][2] * vert0.z) + texinfoArray[texInfoIndex].textureVecs[0][3];
		uv0[1] = (texinfoArray[texInfoIndex].textureVecs[1][0] * vert0.x) + (texinfoArray[texInfoIndex].textureVecs[1][1] * vert0.y) +
				 (texinfoArray[texInfoIndex].textureVecs[1][2] * vert0.z) + texinfoArray[texInfoIndex].textureVecs[1][3];

		lightuv0[0] = (texinfoArray[texInfoIndex].lightmapVecs[0][0] * vert0.x) + (texinfoArray[texInfoIndex].lightmapVecs[0][1] * vert0.y) +
					  (texinfoArray[texInfoIndex].lightmapVecs[0][2] * vert0.z) + texinfoArray[texInfoIndex].lightmapVecs[0][3];
		lightuv0[1] = (texinfoArray[texInfoIndex].lightmapVecs[1][0] * vert0.x) + (texinfoArray[texInfoIndex].lightmapVecs[1][1] * vert0.y) +
					  (texinfoArray[texInfoIndex].lightmapVecs[1][2] * vert0.z) + texinfoArray[texInfoIndex].lightmapVecs[1][3];

		edgeID = surfedgeArray[faceArray[i].firstedge+1].edgeID;
		if (edgeID > 0) {
			// Get first vertex
			vert1 = vertexArray[edgeArray[abs(edgeID)].v[0]];
		}
		else
		{
			// Get second vertex
			vert1 = vertexArray[edgeArray[abs(edgeID)].v[1]];
		}
	
		uv1[0] = (texinfoArray[texInfoIndex].textureVecs[0][0] * vert1.x) + (texinfoArray[texInfoIndex].textureVecs[0][1] * vert1.y) +
				 (texinfoArray[texInfoIndex].textureVecs[0][2] * vert1.z) + texinfoArray[texInfoIndex].textureVecs[0][3];
		uv1[1] = (texinfoArray[texInfoIndex].textureVecs[1][0] * vert1.x) + (texinfoArray[texInfoIndex].textureVecs[1][1] * vert1.y) +
				 (texinfoArray[texInfoIndex].textureVecs[1][2] * vert1.z) + texinfoArray[texInfoIndex].textureVecs[1][3];

		lightuv1[0] = (texinfoArray[texInfoIndex].lightmapVecs[0][0] * vert1.x) + (texinfoArray[texInfoIndex].lightmapVecs[0][1] * vert1.y) +
					  (texinfoArray[texInfoIndex].lightmapVecs[0][2] * vert1.z) + texinfoArray[texInfoIndex].lightmapVecs[0][3];
		lightuv1[1] = (texinfoArray[texInfoIndex].lightmapVecs[1][0] * vert1.x) + (texinfoArray[texInfoIndex].lightmapVecs[1][1] * vert1.y) +
					  (texinfoArray[texInfoIndex].lightmapVecs[1][2] * vert1.z) + texinfoArray[texInfoIndex].lightmapVecs[1][3];

		for (int k = 2; k < faceArray[i].numedges; k++) {

			edgeID = surfedgeArray[faceArray[i].firstedge + k].edgeID;
			if (edgeID > 0) {
				// Get first vertex
				vert2 = vertexArray[edgeArray[abs(edgeID)].v[0]];
			}
			else
			{
				// Get second vertex
				vert2 = vertexArray[edgeArray[abs(edgeID)].v[1]];
			}

			uv2[0] = (texinfoArray[texInfoIndex].textureVecs[0][0] * vert2.x) + (texinfoArray[texInfoIndex].textureVecs[0][1] * vert2.y) +
					 (texinfoArray[texInfoIndex].textureVecs[0][2] * vert2.z) + texinfoArray[texInfoIndex].textureVecs[0][3];
			uv2[1] = (texinfoArray[texInfoIndex].textureVecs[1][0] * vert2.x) + (texinfoArray[texInfoIndex].textureVecs[1][1] * vert2.y) +
					 (texinfoArray[texInfoIndex].textureVecs[1][2] * vert2.z) + texinfoArray[texInfoIndex].textureVecs[1][3];

			lightuv2[0] = (texinfoArray[texInfoIndex].lightmapVecs[0][0] * vert2.x) + (texinfoArray[texInfoIndex].lightmapVecs[0][1] * vert2.y) +
						  (texinfoArray[texInfoIndex].lightmapVecs[0][2] * vert2.z) + texinfoArray[texInfoIndex].lightmapVecs[0][3];
			lightuv2[1] = (texinfoArray[texInfoIndex].lightmapVecs[1][0] * vert2.x) + (texinfoArray[texInfoIndex].lightmapVecs[1][1] * vert2.y) +
						  (texinfoArray[texInfoIndex].lightmapVecs[1][2] * vert2.z) + texinfoArray[texInfoIndex].lightmapVecs[1][3];

			triData.push_back(vert0.x);
			triData.push_back(vert0.y);
			triData.push_back(vert0.z);
			triData.push_back(vert1.x);
			triData.push_back(vert1.y);
			triData.push_back(vert1.z);
			triData.push_back(vert2.x);
			triData.push_back(vert2.y);
			triData.push_back(vert2.z);

			uvData.push_back(uv0[0]);
			uvData.push_back(uv0[1]);
			uvData.push_back(uv1[0]);
			uvData.push_back(uv1[1]);
			uvData.push_back(uv2[0]);
			uvData.push_back(uv2[1]);

			lightmap_uvData.push_back(lightuv0[0]);
			lightmap_uvData.push_back(lightuv0[1]);
			lightmap_uvData.push_back(lightuv1[0]);
			lightmap_uvData.push_back(lightuv1[1]);
			lightmap_uvData.push_back(lightuv2[0]);
			lightmap_uvData.push_back(lightuv2[1]);

			vert1 = vert2;
			uv1[0] = uv2[0];
			uv1[1] = uv2[1];
			lightuv1[0] = lightuv2[0];
			lightuv1[1] = lightuv2[1];
		}
	}
}

GLuint bspHandler::initLightmap()
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	return textureID;
}

void bspHandler::printLump()
{
	std::cout << map_header.ident << "\n";
	std::cout << "[header] BSP version: " << map_header.version << "\n";

	std::cout << "[header] [LUMP 0] Entity Position: " << map_header.lumps[0].fileofs << "\n";
	std::cout << "[header] [LUMP 0] Entity Size: " << map_header.lumps[0].filelen << "\n";
	std::cout << "[header] [LUMP 1] Plane Position: " << map_header.lumps[1].fileofs << "\n";
	std::cout << "[header] [LUMP 1] Plane Size: " << map_header.lumps[1].filelen << "\n";
	std::cout << "[header] [LUMP 2] Texdata Position: " << map_header.lumps[2].fileofs << "\n";
	std::cout << "[header] [LUMP 2] Texdata Size: " << map_header.lumps[2].filelen << "\n";
	std::cout << "[header] [LUMP 3] Vertex Position: " << map_header.lumps[3].fileofs << "\n";
	std::cout << "[header] [LUMP 3] Vertex Size: " << map_header.lumps[3].filelen << "\n";
	std::cout << "[header] [LUMP 4] Visibility Position: " << map_header.lumps[4].fileofs << "\n";
	std::cout << "[header] [LUMP 4] Visibility Size: " << map_header.lumps[4].filelen << "\n";
	std::cout << "[header] [LUMP 5] Node Position: " << map_header.lumps[5].fileofs << "\n";
	std::cout << "[header] [LUMP 5] Mode Size: " << map_header.lumps[5].filelen << "\n";
	std::cout << "[header] [LUMP 6] Texinfo Position: " << map_header.lumps[6].fileofs << "\n";
	std::cout << "[header] [LUMP 6] Texinfo Size: " << map_header.lumps[6].filelen << "\n";
	std::cout << "[header] [LUMP 7] Face Position: " << map_header.lumps[7].fileofs << "\n";
	std::cout << "[header] [LUMP 7] Face Size: " << map_header.lumps[7].filelen << "\n";
	std::cout << "[header] [LUMP 8] Lighting Position: " << map_header.lumps[8].fileofs << "\n";
	std::cout << "[header] [LUMP 8] Lighting Size: " << map_header.lumps[8].filelen << "\n";
	std::cout << "[header] [LUMP 9] Occlusion Position: " << map_header.lumps[9].fileofs << "\n";
	std::cout << "[header] [LUMP 9] Occlusion Size: " << map_header.lumps[9].filelen << "\n";
	std::cout << "[header] [LUMP 10] Leaf Position: " << map_header.lumps[10].fileofs << "\n";
	std::cout << "[header] [LUMP 10] Leaf Size: " << map_header.lumps[10].filelen << "\n";
	std::cout << "[header] [LUMP 11] Faceid Position: " << map_header.lumps[11].fileofs << "\n";
	std::cout << "[header] [LUMP 11] Faceid Size: " << map_header.lumps[11].filelen << "\n";
	std::cout << "[header] [LUMP 12] Edge Position: " << map_header.lumps[12].fileofs << "\n";
	std::cout << "[header] [LUMP 12] Edge Size: " << map_header.lumps[12].filelen << "\n";
	std::cout << "[header] [LUMP 13] Surfedge Position: " << map_header.lumps[13].fileofs << "\n";
	std::cout << "[header] [LUMP 13] Surfedge Size: " << map_header.lumps[13].filelen << "\n";
}

void bspHandler::printVertex()
{
	for (int i = 0; i < vertexArrayLength; i++) {
		std::cout << "[vertex] X:";
		std::cout << vertexArray[i].x;
		std::cout << " Y: ";
		std::cout << vertexArray[i].y;
		std::cout << " Z: ";
		std::cout << vertexArray[i].z;
		std::cout << "\n";
	}
}

void bspHandler::printSurfedge()
{
	for (int i = 0; i < surfedgeArrayLength; i++) {
		std::cout << "[surfedge] edgeID:";
		std::cout << surfedgeArray[i].edgeID;
		std::cout << "\n";
	}
}

void bspHandler::printEdge()
{
	for (int i = 0; i < edgeArrayLength; i++) {
		std::cout << "[edge " << i << "] [" << edgeArray[i].v[0] << ", " << edgeArray[i].v[1] << "] [X: ";
		std::cout << vertexArray[edgeArray[i].v[0]].x;
		std::cout << " Y: ";
		std::cout << vertexArray[edgeArray[i].v[0]].y;
		std::cout << " Z: ";
		std::cout << vertexArray[edgeArray[i].v[0]].z;
		std::cout << "] [X: ";
		std::cout << vertexArray[edgeArray[i].v[1]].x;
		std::cout << " Y: ";
		std::cout << vertexArray[edgeArray[i].v[1]].y;
		std::cout << " Z: ";
		std::cout << vertexArray[edgeArray[i].v[1]].z;
		std::cout << "]\n";
	}
}

void bspHandler::printFace()
{

}

void bspHandler::printTris()
{
	/*
	for (int j = 0; j < mapLength / 9; j++)
	{
		std::cout << "[triangle " << j << "]\n";
		std::cout << "X: " << (float)mapData[j * 9] << " Y: " << (float)mapData[j * 9 + 1] << " Z: " << (float)mapData[j * 9 + 2] << "\n";
		std::cout << "X: " << (float)mapData[j * 9 + 3] << " Y: " << (float)mapData[j * 9 + 4] << " Z: " << (float)mapData[j * 9 + 5] << "\n";
		std::cout << "X: " << (float)mapData[j * 9 + 6] << " Y: " << (float)mapData[j * 9 + 7] << " Z: " << (float)mapData[j * 9 + 8] << "\n";
	}
	*/
}

