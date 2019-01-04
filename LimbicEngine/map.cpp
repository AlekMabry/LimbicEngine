#include "map.hpp"

map::map(render* rHandle)
{
	renderHandle = rHandle;
}

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

	renderHandle->openWAD(wadname);

	for (int i = 0; i < texHeader; i++)
	{
		int id;
		int status = renderHandle->addMatWAD(texdataArray[i].szName, vec2(texdataArray[i].nWidth, texdataArray[i].nHeight),
			id, 0);
		materialIndex.push_back(id);
		if (status != STATUS_SUCCESS)
		{
			std::cout << "[ERROR] Can't find texture: " << texdataArray[i].szName << ", defaulting to tex_missing \n";
		}
	}

	renderHandle->closeWAD();

	return STATUS_SUCCESS;
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

	gl_vertexBuffers.clear();
	gl_uvBuffers.clear();

	/*
	The renderer draws regular opaque surfaces first then draws transparent surfaces like glass.
	The vertices themselves will be stored in a 2D vector array, where each row represents a texture index.
	When rendering, the renderer will go through this array and set the texture before drawing.
	*/

	// Iterate through all possible textures that can be drawn
	for (int texID = 0; texID < texHeader; texID++)
	{
		std::vector<float> temp_vertData;
		std::vector<float> temp_uvData;

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

					temp_vertData.push_back(vert0.x);
					temp_vertData.push_back(vert0.y);
					temp_vertData.push_back(vert0.z);
					temp_vertData.push_back(vert1.x);
					temp_vertData.push_back(vert1.y);
					temp_vertData.push_back(vert1.z);
					temp_vertData.push_back(vert2.x);
					temp_vertData.push_back(vert2.y);
					temp_vertData.push_back(vert2.z);

					temp_uvData.push_back(uv0[0]);
					temp_uvData.push_back(uv0[1]);
					temp_uvData.push_back(uv1[0]);
					temp_uvData.push_back(uv1[1]);
					temp_uvData.push_back(uv2[0]);
					temp_uvData.push_back(uv2[1]);

					vert1 = vert2;
					uv1[0] = uv2[0];
					uv1[1] = uv2[1];
				}
			}
		}
		gl_vertexBuffers.push_back(temp_vertData);
		gl_uvBuffers.push_back(temp_uvData);
	}
}

void map::pushBuffers()
{
	for (int i = 0; i < texHeader; i++)
	{
		if (gl_vertexBuffers[i].size() != 0)
		{
			renderHandle->pushVertices(materialIndex[i], gl_vertexBuffers[i], gl_uvBuffers[i]);
		}
	}
}

void map::setRenderHandle(render* handle)
{
	renderHandle = handle;
}