#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

#include "bspHandler.hpp"

int bspHandler::loadBSP(const char *fname, bool isGldSrc) 
{
	gldSrc = isGldSrc;

	FILE* mapFile;

	if (fopen_s(&mapFile, fname, "rb"))
	{
		fprintf(stderr, "[error] BSP failed to open!\n");
		return 1;
	}

	if (!gldSrc)
	{
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
	}
	else
	{
		// Load the header into memory so we can analyze it
		fread(&map_header_gld, sizeof(map_header_gld), 1, mapFile);

		int arrayLength;

		// Load vertexes
		arrayLength = (int)((float)map_header_gld.lumps[3].nLength / (float) sizeof(Vector));
		vertexArray = new Vector[arrayLength];
		vertexArrayLength = arrayLength;
		fseek(mapFile, map_header_gld.lumps[3].nOffset, SEEK_SET);
		for (int i = 0; i < vertexArrayLength; i++) {
			fread(&vertexArray[i], sizeof(Vector), 1, mapFile);
		}

		// Load texinfo
		arrayLength = (int)((float)map_header_gld.lumps[6].nLength / (float) sizeof(dtexinfo_gld_t));
		texinfoArray_gld = new dtexinfo_gld_t[arrayLength];
		texinfoArrayLength = arrayLength;
		fseek(mapFile, map_header_gld.lumps[6].nOffset, SEEK_SET);
		for (int i = 0; i < texinfoArrayLength; i++) {
			fread(&texinfoArray_gld[i], sizeof(dtexinfo_gld_t), 1, mapFile);
		}

		// Load texture pointers
		fseek(mapFile, map_header_gld.lumps[2].nOffset, SEEK_SET);
		fread(&texoffsetLength, sizeof(unsigned int), 1, mapFile);
		texoffsetArray_gld = new dmiptexoffset_gld_t[texoffsetLength];
		for (unsigned int i = 0; i < texoffsetLength; i++)
		{
			fread(&texoffsetArray_gld[i], sizeof(dmiptexoffset_gld_t), 1, mapFile);
		}
		texpointerArray_gld = new dmiptex_gld_t[texoffsetLength];
		for (unsigned int i = 0; i < texoffsetLength; i++)
		{
			fseek(mapFile, texoffsetArray_gld[i] + map_header_gld.lumps[2].nOffset, SEEK_SET);
			fread(&texpointerArray_gld[i], sizeof(dmiptex_gld_t), 1, mapFile);
		}

		// Load edges
		arrayLength = (int)((float)map_header_gld.lumps[12].nLength / (float) sizeof(dedge_t));
		edgeArray = (dedge_t*)calloc(arrayLength, sizeof(dedge_t)); // new dedge_t[arrayLength];
		edgeArrayLength = arrayLength;
		fseek(mapFile, map_header_gld.lumps[12].nOffset, SEEK_SET);
		for (int i = 0; i < edgeArrayLength; i++) {
			fread(&edgeArray[i], sizeof(dedge_t), 1, mapFile);
		}

		// Load surfedges
		arrayLength = (int)((float)map_header_gld.lumps[13].nLength / (float) sizeof(dsurfedge_t));
		surfedgeArray = new dsurfedge_t[arrayLength];
		surfedgeArrayLength = arrayLength;
		fseek(mapFile, map_header_gld.lumps[13].nOffset, SEEK_SET);
		for (int i = 0; i < surfedgeArrayLength; i++) {
			fread(&surfedgeArray[i], sizeof(dsurfedge_t), 1, mapFile);
		}

		// Load face
		arrayLength = (int)((float)map_header_gld.lumps[7].nLength / (float) sizeof(dface_gld_t));
		faceArray_gld = new dface_gld_t[arrayLength];
		faceArrayLength = arrayLength;
		fseek(mapFile, map_header_gld.lumps[7].nOffset, SEEK_SET);
		for (int i = 0; i < faceArrayLength; i++) {
			fread(&faceArray_gld[i], sizeof(dface_gld_t), 1, mapFile);
		}
	}

	if (mapFile)
	{
		if (fclose(mapFile))
		{
			fprintf(stderr, "[error] BSP failed to close!\n");
		}
	}

	if (fopen_s(&mapFile, "outpost731/textures/halflife.wad", "rb"))
	{
		fprintf(stderr, "[error] WAD failed to open!\n");
		return 1;
	}

	// Load WAD header
	fread(&tex_header_wad, sizeof(tex_header_wad), 1, mapFile);

	int arrayLength;

	// Load all of the entries in the directory
	arrayLength = tex_header_wad.nDir;
	entryArray_wad = new dentry_wad_t[arrayLength];
	entryArrayLength = arrayLength;
	fseek(mapFile, tex_header_wad.nDirOffset, SEEK_SET);
	for (int i = 0; i < entryArrayLength; i++) {
		fread(&entryArray_wad[i], sizeof(dentry_wad_t), 1, mapFile);
	}

	// Load textures into openGL if they are in the WAD file instead
	gl_maptextures = new GLuint[texoffsetLength];
	glGenTextures(texoffsetLength, &gl_maptextures[0]);
	for (int i = 0; i < texoffsetLength; i++)
	{
		// First find the texture itself in the WAD entry list
		int texID_wad = -1;
		for (int j = 0; j < entryArrayLength; j++)
		{
			// Upper Case Nonsense
			/*
			char *strA = _strdup(entryArray_wad[j].szName);
			for (int k = 0; k < 16; k++)
			{
				if (strA[k] >= 97 && strA[k] <= 122)
					strA[k] -= 32;
			}
			char *strB = _strdup(texpointerArray_gld[i].szName);
			for (int k = 0; k < 16; k++)
			{
				if (strB[k] >= 97 && strB[k] <= 122)
					strB[k] -= 32;
			}
			*/
			std::string strA = _strdup(entryArray_wad[j].szName);
			std::string strB = _strdup(texpointerArray_gld[i].szName);
			for (int k = 0; k < 16; k++)
			{
				strA[k] = toupper(strA[k]);
				strB[k] = toupper(strB[k]);
			}

			// Compare
			if (strA == strB)
			{
				texID_wad = j;
			}
		}
		if (texID_wad == -1)
		{
			std::cout << "[CRITICAL ERROR] Texture " << texpointerArray_gld[i].szName << " not found! \n";
		}
		else
		{
			// Now load that texture info
			dmiptex_gld_t wadTexInfo;
			fseek(mapFile, entryArray_wad[texID_wad].nFilePos, SEEK_SET);
			fread(&wadTexInfo, sizeof(dmiptex_gld_t), 1, mapFile);

			// Print if the file is compressed
			std::cout << "[WAD] Texture " << wadTexInfo.szName << " compression state is:" << entryArray_wad[texID_wad].bCompression << "\n";

			int totalPixels = wadTexInfo.nWidth * wadTexInfo.nHeight;
			unsigned char* data = new unsigned char[totalPixels * 3];

			for (int j = 0; j < totalPixels; j++)
			{
				/*
				The texture consists of four mipmap levels, with each pixel represented as a byte
				that points to a 24-bit color in a color pallate

				The offset to the texture is dmiptex_gld_t(Position in WAD) + dmiptex_gld_t.nOffset[0]
				*/
				fseek(mapFile, entryArray_wad[texID_wad].nFilePos + wadTexInfo.nOffsets[0] + j, SEEK_SET);
				unsigned char currentColor;
				fread(&currentColor, sizeof(unsigned char), 1, mapFile);

				// Find what that color actually is by looking it up in the table
				// The table position is at the end of the fourth mipmap after two dummy bytes
				fseek(mapFile, entryArray_wad[texID_wad].nFilePos + wadTexInfo.nOffsets[3] +
					(totalPixels / 64) + 2 + ((int)currentColor * 3), SEEK_SET);
				unsigned char red;
				unsigned char green;
				unsigned char blue;
				fread(&red, sizeof(unsigned char), 1, mapFile);
				fread(&green, sizeof(unsigned char), 1, mapFile);
				fread(&blue, sizeof(unsigned char), 1, mapFile);
				data[(j * 3)] = red;
				data[(j * 3) + 1] = green;
				data[(j * 3) + 2] = blue;
			}

			glBindTexture(GL_TEXTURE_2D, gl_maptextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texpointerArray_gld[i].nWidth,
				texpointerArray_gld[i].nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			delete[] data;
		}
	}

	if (mapFile)
	{
		if (fclose(mapFile))
		{
			fprintf(stderr, "[error] WAD failed to close!\n");
		}
	}

	// Create the VAOs (Vertex Array Objects)
	glGenVertexArrays(1, &gl_vao);
	glBindVertexArray(gl_vao);

	// Create buffers (VBO)
	glGenBuffers(1, &gl_vertexData);
	glGenBuffers(1, &gl_uvData);


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

	if (!gldSrc)
	{
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

			edgeID = surfedgeArray[faceArray[i].firstedge + 1].edgeID;
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
	else
	{
		for (int i = 0; i < map_header_gld.lumps[7].nLength / sizeof(dface_gld_t); i++) {
			short texInfoIndex = faceArray_gld[i].iTextureInfo;

			signed int edgeID = surfedgeArray[faceArray_gld[i].iFirstEdge].edgeID;
			if (edgeID > 0) {
				// Get first vertex
				vert0 = vertexArray[edgeArray[abs(edgeID)].v[0]];
			}
			else
			{
				// Get second vertex
				vert0 = vertexArray[edgeArray[abs(edgeID)].v[1]];
			}

			uv0[0] = (texinfoArray_gld[texInfoIndex].vS.x * vert0.x) + (texinfoArray_gld[texInfoIndex].vS.y * vert0.y) +
				(texinfoArray_gld[texInfoIndex].vS.z * vert0.z) + texinfoArray_gld[texInfoIndex].fSShift;
			uv0[1] = (texinfoArray_gld[texInfoIndex].vT.x * vert0.x) + (texinfoArray_gld[texInfoIndex].vT.y * vert0.y) +
				(texinfoArray_gld[texInfoIndex].vT.z * vert0.z) + texinfoArray_gld[texInfoIndex].fTShift;

			edgeID = surfedgeArray[faceArray_gld[i].iFirstEdge + 1].edgeID;
			if (edgeID > 0) {
				// Get first vertex
				vert1 = vertexArray[edgeArray[abs(edgeID)].v[0]];
			}
			else
			{
				// Get second vertex
				vert1 = vertexArray[edgeArray[abs(edgeID)].v[1]];
			}

			uv1[0] = (texinfoArray_gld[texInfoIndex].vS.x * vert1.x) + (texinfoArray_gld[texInfoIndex].vS.y * vert1.y) +
				(texinfoArray_gld[texInfoIndex].vS.z * vert1.z) + texinfoArray_gld[texInfoIndex].fSShift;
			uv1[1] = (texinfoArray_gld[texInfoIndex].vT.x * vert1.x) + (texinfoArray_gld[texInfoIndex].vT.y * vert1.y) +
				(texinfoArray_gld[texInfoIndex].vT.z * vert1.z) + texinfoArray_gld[texInfoIndex].fTShift;

			for (int k = 2; k < faceArray_gld[i].nEdges; k++) {

				edgeID = surfedgeArray[faceArray_gld[i].iFirstEdge + k].edgeID;
				if (edgeID > 0) {
					// Get first vertex
					vert2 = vertexArray[edgeArray[abs(edgeID)].v[0]];
				}
				else
				{
					// Get second vertex
					vert2 = vertexArray[edgeArray[abs(edgeID)].v[1]];
				}

				uv2[0] = (texinfoArray_gld[texInfoIndex].vS.x * vert2.x) + (texinfoArray_gld[texInfoIndex].vS.y * vert2.y) +
					(texinfoArray_gld[texInfoIndex].vS.z * vert2.z) + texinfoArray_gld[texInfoIndex].fSShift;
				uv2[1] = (texinfoArray_gld[texInfoIndex].vT.x * vert2.x) + (texinfoArray_gld[texInfoIndex].vT.y * vert2.y) +
					(texinfoArray_gld[texInfoIndex].vT.z * vert2.z) + texinfoArray_gld[texInfoIndex].fTShift;

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

				vert1 = vert2;
				uv1[0] = uv2[0];
				uv1[1] = uv2[1];
			}
		}
	}
}

void bspHandler::compileBuffers()
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

	if (!gldSrc)
	{
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

			edgeID = surfedgeArray[faceArray[i].firstedge + 1].edgeID;
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

				gl_vertexBuffer.push_back(vert0.x);
				gl_vertexBuffer.push_back(vert0.y);
				gl_vertexBuffer.push_back(vert0.z);
				gl_vertexBuffer.push_back(vert1.x);
				gl_vertexBuffer.push_back(vert1.y);
				gl_vertexBuffer.push_back(vert1.z);
				gl_vertexBuffer.push_back(vert2.x);
				gl_vertexBuffer.push_back(vert2.y);
				gl_vertexBuffer.push_back(vert2.z);

				gl_uvBuffer.push_back(uv0[0]);
				gl_uvBuffer.push_back(uv0[1]);
				gl_uvBuffer.push_back(uv1[0]);
				gl_uvBuffer.push_back(uv1[1]);
				gl_uvBuffer.push_back(uv2[0]);
				gl_uvBuffer.push_back(uv2[1]);

				/*
				lightmap_uvData.push_back(lightuv0[0]);
				lightmap_uvData.push_back(lightuv0[1]);
				lightmap_uvData.push_back(lightuv1[0]);
				lightmap_uvData.push_back(lightuv1[1]);
				lightmap_uvData.push_back(lightuv2[0]);
				lightmap_uvData.push_back(lightuv2[1]);
				*/

				vert1 = vert2;
				uv1[0] = uv2[0];
				uv1[1] = uv2[1];
				lightuv1[0] = lightuv2[0];
				lightuv1[1] = lightuv2[1];
			}
		}
	}
	else
	{
		/*
		The renderer draws regular opaque surfaces first then draws transparent surfaces like glass.
		The vertices themselves will be stored in a 2D vector array, where each row represents a texture index.
		When rendering, the renderer will go through this array and set the texture before drawing.
		*/

		// Iterate through all possible textures that can be drawn
		for (int texID = 0; texID < texoffsetLength; texID++)
		{
			gl_vertexBuffers[texID].clear();
			gl_uvBuffers[texID].clear();

			// Iterate through every face on the map to find if it matches the current texture
			for (int i = 0; i < map_header_gld.lumps[7].nLength / sizeof(dface_gld_t); i++) {
				short texInfoIndex = faceArray_gld[i].iTextureInfo;

				// Check to see if current face matches the current texture
				if (texinfoArray_gld[texInfoIndex].iMiptex == texID)
				{
					signed int edgeID = surfedgeArray[faceArray_gld[i].iFirstEdge].edgeID;
					if (edgeID > 0) {
						// Get first vertex
						vert0 = vertexArray[edgeArray[abs(edgeID)].v[0]];
					}
					else
					{
						// Get second vertex
						vert0 = vertexArray[edgeArray[abs(edgeID)].v[1]];
					}

					uv0[0] = (texinfoArray_gld[texInfoIndex].vS.x * vert0.x) + (texinfoArray_gld[texInfoIndex].vS.y * vert0.y) +
						(texinfoArray_gld[texInfoIndex].vS.z * vert0.z) + texinfoArray_gld[texInfoIndex].fSShift;
					uv0[1] = (texinfoArray_gld[texInfoIndex].vT.x * vert0.x) + (texinfoArray_gld[texInfoIndex].vT.y * vert0.y) +
						(texinfoArray_gld[texInfoIndex].vT.z * vert0.z) + texinfoArray_gld[texInfoIndex].fTShift;

					edgeID = surfedgeArray[faceArray_gld[i].iFirstEdge + 1].edgeID;
					if (edgeID > 0) {
						// Get first vertex
						vert1 = vertexArray[edgeArray[abs(edgeID)].v[0]];
					}
					else
					{
						// Get second vertex
						vert1 = vertexArray[edgeArray[abs(edgeID)].v[1]];
					}

					uv1[0] = (texinfoArray_gld[texInfoIndex].vS.x * vert1.x) + (texinfoArray_gld[texInfoIndex].vS.y * vert1.y) +
						(texinfoArray_gld[texInfoIndex].vS.z * vert1.z) + texinfoArray_gld[texInfoIndex].fSShift;
					uv1[1] = (texinfoArray_gld[texInfoIndex].vT.x * vert1.x) + (texinfoArray_gld[texInfoIndex].vT.y * vert1.y) +
						(texinfoArray_gld[texInfoIndex].vT.z * vert1.z) + texinfoArray_gld[texInfoIndex].fTShift;

					for (int k = 2; k < faceArray_gld[i].nEdges; k++) {

						edgeID = surfedgeArray[faceArray_gld[i].iFirstEdge + k].edgeID;
						if (edgeID > 0) {
							// Get first vertex
							vert2 = vertexArray[edgeArray[abs(edgeID)].v[0]];
						}
						else
						{
							// Get second vertex
							vert2 = vertexArray[edgeArray[abs(edgeID)].v[1]];
						}

						uv2[0] = (texinfoArray_gld[texInfoIndex].vS.x * vert2.x) + (texinfoArray_gld[texInfoIndex].vS.y * vert2.y) +
							(texinfoArray_gld[texInfoIndex].vS.z * vert2.z) + texinfoArray_gld[texInfoIndex].fSShift;
						uv2[1] = (texinfoArray_gld[texInfoIndex].vT.x * vert2.x) + (texinfoArray_gld[texInfoIndex].vT.y * vert2.y) +
							(texinfoArray_gld[texInfoIndex].vT.z * vert2.z) + texinfoArray_gld[texInfoIndex].fTShift;



						gl_vertexBuffers[texID].push_back(vert0.x);
						gl_vertexBuffers[texID].push_back(vert0.y);
						gl_vertexBuffers[texID].push_back(vert0.z);
						gl_vertexBuffers[texID].push_back(vert1.x);
						gl_vertexBuffers[texID].push_back(vert1.y);
						gl_vertexBuffers[texID].push_back(vert1.z);
						gl_vertexBuffers[texID].push_back(vert2.x);
						gl_vertexBuffers[texID].push_back(vert2.y);
						gl_vertexBuffers[texID].push_back(vert2.z);

						gl_uvBuffers[texID].push_back(uv0[0]);
						gl_uvBuffers[texID].push_back(uv0[1]);
						gl_uvBuffers[texID].push_back(uv1[0]);
						gl_uvBuffers[texID].push_back(uv1[1]);
						gl_uvBuffers[texID].push_back(uv2[0]);
						gl_uvBuffers[texID].push_back(uv2[1]);

						vert1 = vert2;
						uv1[0] = uv2[0];
						uv1[1] = uv2[1];
					}
				}
			}
		}
	}
}

void bspHandler::renderMap()
{
	compileBuffers();

	for (int i = texoffsetLength; i > 0; i--)
	{
		// Bind the VAOs (Vertex Array Objects)
		glBindVertexArray(gl_vao);

		// Bind buffers (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, gl_vertexData);
		glBufferData(GL_ARRAY_BUFFER, gl_vertexBuffers[i].size() * sizeof(GLfloat), &gl_vertexBuffers[i][0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, gl_uvData);
		glBufferData(GL_ARRAY_BUFFER, gl_uvBuffers[i].size() * sizeof(GLfloat), &gl_uvBuffers[i][0], GL_STATIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, gl_maptextures[i]);

		// Finally draw
		renderHandle->drawTris(1, gl_vertexBuffers[i].size(), gl_vertexData, gl_uvData,
			texpointerArray_gld[i].nWidth, texpointerArray_gld[i].nHeight);
		// renderHandle->drawTris(1, vertPoints.size(), gl_vertexData, gl_uvData);
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

void bspHandler::setRenderPointer(render* renderPointer)
{
	renderHandle = renderPointer;
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

void bspHandler::printTexPointers()
{
	for (int j = 0; j < texinfoArrayLength; j++)
	{
		//std::cout << "[Texture Pointer] #" << j << " &" << texoffsetArray_gld[j] << "\n";
		std::cout << "[TEXINFO] iMiptex:" << texinfoArray_gld[j].iMiptex << "\n";
	}

	std::cout << "[VERTEX BUFFER SIZE] " << gl_vertexBuffers[0].size() << "\n";
	std::cout << "[VERTEX BUFFER SIZE] " << gl_vertexBuffers[8].size() << "\n";
	std::cout << "[VERTEX BUFFER SIZE] " << gl_vertexBuffers[9].size() << "\n";
}

void bspHandler::printTexInfo()
{
	for (int i = 0; i < texoffsetLength; i++)
	{
		std::cout << "[TEXINFO] W:" << texpointerArray_gld[i].nWidth;
		std::cout << " H:" << texpointerArray_gld[i].nHeight;
		std::cout << " NAME:" << texpointerArray_gld[i].szName;
		std::cout << "OFFSET: [0: " << texpointerArray_gld[i].nOffsets[0];
		std::cout << " 1:" << texpointerArray_gld[i].nOffsets[1];
		std::cout << " 2:" << texpointerArray_gld[i].nOffsets[2];
		std::cout << " 3:" << texpointerArray_gld[i].nOffsets[3] << "]\n";;
	}
}