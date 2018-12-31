#include "map.hpp"

int map::load(const char *bspname, const char *wadname)
{
	FILE* bspFile;

	if (fopen_s(&bspFile, bspname, "rb"))
	{
		fprintf(stderr, "[ERROR] BSP failed to open!\n");
	}

	// Load the header into memory so we can analyze it
	fread(&bspHeader, sizeof(dbspheader_t), 1, bspFile);

	// Load entities

	// Load planes
	planeArrayLen = bspHeader.lump[LUMP_PLANES].nLength / sizeof(dplane_t);
	planeArray = new dplane_t[planeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_PLANES].nOffset, SEEK_SET);
	for (int i = 0; i < planeArrayLen; i++) {
		fread(&planeArray[i], sizeof(dplane_t), 1, bspFile);
	}

	// Load textures (pointers)
	fseek(bspFile, bspHeader.lump[LUMP_TEXTURES].nOffset, SEEK_SET);
	fread(&texHeader, sizeof(dtexheader_t), 1, bspFile);
	texoffsetArray = new dtexoffset_t[texHeader];
	for (unsigned int i = 0; i < texHeader; i++)
	{
		fread(&texoffsetArray[i], sizeof(dtexoffset_t), 1, bspFile);
	}
	texdataArray = new dtexdata_t[texHeader];
	for (unsigned int i = 0; i < texHeader; i++)
	{
		fseek(bspFile, texoffsetArray[i] + bspHeader.lump[LUMP_TEXTURES].nOffset, SEEK_SET);
		fread(&texdataArray[i], sizeof(dtexdata_t), 1, bspFile);
	}

	// Load vertexes
	vertexArrayLen = bspHeader.lump[LUMP_VERTICES].nLength / sizeof(dvertex_t);
	vertexArray = new dvertex_t[vertexArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_VERTICES].nOffset, SEEK_SET);
	for (int i = 0; i < vertexArrayLen; i++) {
		fread(&vertexArray[i], sizeof(dvertex_t), 1, bspFile);
	}

	// Load visibility

	// Load nodes
	nodeArrayLen = bspHeader.lump[LUMP_NODES].nLength / sizeof(dnode_t);
	nodeArray = new dnode_t[nodeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_NODES].nOffset, SEEK_SET);
	for (int i = 0; i < nodeArrayLen; i++) {
		fread(&nodeArray[i], sizeof(dnode_t), 1, bspFile);
	}

	// Load texinfo
	texinfoArrayLen = bspHeader.lump[LUMP_TEXINFO].nLength / sizeof(dtexinfo_t);
	texinfoArray = new dtexinfo_t[texinfoArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_TEXINFO].nOffset, SEEK_SET);
	for (int i = 0; i < texinfoArrayLen; i++) {
		fread(&texinfoArray[i], sizeof(dtexinfo_t), 1, bspFile);
	}

	// Load faces
	faceArrayLen = bspHeader.lump[LUMP_FACES].nLength / sizeof(dface_t);
	faceArray = new dface_t[faceArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_FACES].nOffset, SEEK_SET);
	for (int i = 0; i < faceArrayLen; i++) {
		fread(&faceArray[i], sizeof(dface_t), 1, bspFile);
	}

	// Load lighting

	// Load clipnodes
	clipnodeArrayLen = bspHeader.lump[LUMP_CLIPNODES].nLength / sizeof(dclipnode_t);
	clipnodeArray = new dclipnode_t[clipnodeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_FACES].nOffset, SEEK_SET);
	for (int i = 0; i < faceArrayLen; i++) {
		fread(&faceArray[i], sizeof(dface_t), 1, bspFile);
	}

	// Load leaves
	leafArrayLen = bspHeader.lump[LUMP_LEAVES].nLength / sizeof(dleave_t);
	leafArray = new dleave_t[leafArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_LEAVES].nOffset, SEEK_SET);
	for (int i = 0; i < leafArrayLen; i++) {
		fread(&leafArray[i], sizeof(dleave_t), 1, bspFile);
	}

	// Load marksurfaces
	clipnodeArrayLen = bspHeader.lump[LUMP_CLIPNODES].nLength / sizeof(dclipnode_t);
	clipnodeArray = new dclipnode_t[clipnodeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_CLIPNODES].nOffset, SEEK_SET);
	for (int i = 0; i < clipnodeArrayLen; i++) {
		fread(&clipnodeArray[i], sizeof(dclipnode_t), 1, bspFile);
	}

	// Load edges
	edgeArrayLen = bspHeader.lump[LUMP_EDGES].nLength / sizeof(dedge_t);
	edgeArray = new dedge_t[edgeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_EDGES].nOffset, SEEK_SET);
	for (int i = 0; i < edgeArrayLen; i++) {
		fread(&edgeArray[i], sizeof(dedge_t), 1, bspFile);
	}

	// Load surfedges
	surfedgeArrayLen = bspHeader.lump[LUMP_SURFEDGES].nLength / sizeof(dsurfedge_t);
	surfedgeArray = new dsurfedge_t[surfedgeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_SURFEDGES].nOffset, SEEK_SET);
	for (int i = 0; i < surfedgeArrayLen; i++) {
		fread(&surfedgeArray[i], sizeof(dsurfedge_t), 1, bspFile);
	}

	// Load surfedges
	surfedgeArrayLen = bspHeader.lump[LUMP_SURFEDGES].nLength / sizeof(dsurfedge_t);
	surfedgeArray = new dsurfedge_t[surfedgeArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_SURFEDGES].nOffset, SEEK_SET);
	for (int i = 0; i < surfedgeArrayLen; i++) {
		fread(&surfedgeArray[i], sizeof(dsurfedge_t), 1, bspFile);
	}

	// Load models
	modelArrayLen = bspHeader.lump[LUMP_MODELS].nLength / sizeof(dmodel_t);
	modelArray = new dmodel_t[modelArrayLen];
	fseek(bspFile, bspHeader.lump[LUMP_MODELS].nOffset, SEEK_SET);
	for (int i = 0; i < modelArrayLen; i++) {
		fread(&modelArray[i], sizeof(dmodel_t), 1, bspFile);
	}

	if (bspFile)
	{
		if (fclose(bspFile))
		{
			fprintf(stderr, "[ERROR] BSP failed to close!\n");
		}
	}

	FILE* wadFile;

	if (fopen_s(&wadFile, wadname, "rb"))
	{
		fprintf(stderr, "[error] WAD failed to open!\n");
	}

	// Load WAD header
	fread(&wadHeader, sizeof(dwadheader_t), 1, wadFile);

	// Load all of the entries in the directory
	wadEntryArrayLen = wadHeader.nDir;
	wadEntryArray = new dentry_t[wadEntryArrayLen];
	fseek(wadFile, wadHeader.nDirOffset, SEEK_SET);
	for (int i = 0; i < wadEntryArrayLen; i++) {
		fread(&wadEntryArray[i], sizeof(dentry_t), 1, wadFile);
	}

	// Load textures into openGL if they are in the WAD file instead
	gl_maptextures = new GLuint[texHeader];
	glGenTextures(texHeader, &gl_maptextures[0]);
	for (int i = 0; i < texHeader; i++)
	{
		// First find the texture itself in the WAD entry list
		int texID_wad = -1;
		for (int j = 0; j < wadEntryArrayLen; j++)
		{
			std::string strA = _strdup(wadEntryArray[j].szName);
			std::string strB = _strdup(texdataArray[i].szName);
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
			std::cout << "[CRITICAL ERROR] Texture " << texdataArray[i].szName << " not found! \n";
		}
		else
		{
			// Now load that texture info
			dtexdata_t wadTexInfo;
			fseek(wadFile, wadEntryArray[texID_wad].nFilePos, SEEK_SET);
			fread(&wadTexInfo, sizeof(dtexdata_t), 1, wadFile);

			int totalPixels = wadTexInfo.nWidth * wadTexInfo.nHeight;
			unsigned char* data = new unsigned char[totalPixels * 3];

			for (int j = 0; j < totalPixels; j++)
			{
				/*
				The texture consists of four mipmap levels, with each pixel represented as a byte
				that points to a 24-bit color in a color pallate

				The offset to the texture is dmiptex_gld_t(Position in WAD) + dmiptex_gld_t.nOffset[0]
				*/
				fseek(wadFile, wadEntryArray[texID_wad].nFilePos + wadTexInfo.nOffsets[0] + j, SEEK_SET);
				unsigned char currentColor;
				fread(&currentColor, sizeof(unsigned char), 1, wadFile);

				// Find what that color actually is by looking it up in the table
				// The table position is at the end of the fourth mipmap after two dummy bytes
				fseek(wadFile, wadEntryArray[texID_wad].nFilePos + wadTexInfo.nOffsets[3] +
					(totalPixels / 64) + 2 + ((int)currentColor * 3), SEEK_SET);
				unsigned char red;
				unsigned char green;
				unsigned char blue;
				fread(&red, sizeof(unsigned char), 1, wadFile);
				fread(&green, sizeof(unsigned char), 1, wadFile);
				fread(&blue, sizeof(unsigned char), 1, wadFile);
				data[(j * 3)] = red;
				data[(j * 3) + 1] = green;
				data[(j * 3) + 2] = blue;
			}

			glBindTexture(GL_TEXTURE_2D, gl_maptextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texdataArray[i].nWidth,
				texdataArray[i].nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			delete[] data;
		}
	}

	if (wadFile)
	{
		if (fclose(wadFile))
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

void map::compileBuffers()
{
	// Generate face array from visibility information

	int edgeIndex, v;
	dvec3_t vert0;
	dvec3_t vert1;
	dvec3_t vert2;

	float uv0[2];
	float uv1[2];
	float uv2[2];

	float lightuv0[2];
	float lightuv1[2];
	float lightuv2[2];

	/*
	The renderer draws regular opaque surfaces first then draws transparent surfaces like glass.
	The vertices themselves will be stored in a 2D vector array, where each row represents a texture index.
	When rendering, the renderer will go through this array and set the texture before drawing.
	*/

	// Iterate through all possible textures that can be drawn
	for (int texID = 0; texID < texHeader; texID++)
	{
		gl_vertexBuffers[texID].clear();
		gl_uvBuffers[texID].clear();

		// Iterate through every face on the map to find if it matches the current texture
		for (int i = 0; i < faceArrayLen; i++) {
			short texInfoIndex = faceArray[i].iTextureInfo;

			// Check to see if current face matches the current texture
			if (texinfoArray[texInfoIndex].iMiptex == texID)
			{
				signed int edgeID = surfedgeArray[faceArray[i].iFirstEdge].iEdgeID;
				if (edgeID > 0) {
					// Get first vertex
					vert0 = vertexArray[edgeArray[abs(edgeID)].iV[0]];
				}
				else
				{
					// Get second vertex
					vert0 = vertexArray[edgeArray[abs(edgeID)].iV[1]];
				}

				uv0[0] = (texinfoArray[texInfoIndex].vS.x * vert0.x) + (texinfoArray[texInfoIndex].vS.y * vert0.y) +
					(texinfoArray[texInfoIndex].vS.z * vert0.z) + texinfoArray[texInfoIndex].fSShift;
				uv0[1] = (texinfoArray[texInfoIndex].vT.x * vert0.x) + (texinfoArray[texInfoIndex].vT.y * vert0.y) +
					(texinfoArray[texInfoIndex].vT.z * vert0.z) + texinfoArray[texInfoIndex].fTShift;

				edgeID = surfedgeArray[faceArray[i].iFirstEdge + 1].iEdgeID;
				if (edgeID > 0) {
					// Get first vertex
					vert1 = vertexArray[edgeArray[abs(edgeID)].iV[0]];
				}
				else
				{
					// Get second vertex
					vert1 = vertexArray[edgeArray[abs(edgeID)].iV[1]];
				}

				uv1[0] = (texinfoArray[texInfoIndex].vS.x * vert1.x) + (texinfoArray[texInfoIndex].vS.y * vert1.y) +
					(texinfoArray[texInfoIndex].vS.z * vert1.z) + texinfoArray[texInfoIndex].fSShift;
				uv1[1] = (texinfoArray[texInfoIndex].vT.x * vert1.x) + (texinfoArray[texInfoIndex].vT.y * vert1.y) +
					(texinfoArray[texInfoIndex].vT.z * vert1.z) + texinfoArray[texInfoIndex].fTShift;

				for (int k = 2; k < faceArray[i].nEdges; k++) {

					edgeID = surfedgeArray[faceArray[i].iFirstEdge + k].iEdgeID;
					if (edgeID > 0) {
						// Get first vertex
						vert2 = vertexArray[edgeArray[abs(edgeID)].iV[0]];
					}
					else
					{
						// Get second vertex
						vert2 = vertexArray[edgeArray[abs(edgeID)].iV[1]];
					}

					uv2[0] = (texinfoArray[texInfoIndex].vS.x * vert2.x) + (texinfoArray[texInfoIndex].vS.y * vert2.y) +
						(texinfoArray[texInfoIndex].vS.z * vert2.z) + texinfoArray[texInfoIndex].fSShift;
					uv2[1] = (texinfoArray[texInfoIndex].vT.x * vert2.x) + (texinfoArray[texInfoIndex].vT.y * vert2.y) +
						(texinfoArray[texInfoIndex].vT.z * vert2.z) + texinfoArray[texInfoIndex].fTShift;

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

void map::renderMap()
{
	compileBuffers();

	for (int i = 0; i < texHeader; i++)
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
			texdataArray[i].nWidth, texdataArray[i].nHeight);
	}
}

void map::setRenderHandle(render* handle)
{
	renderHandle = handle;
}