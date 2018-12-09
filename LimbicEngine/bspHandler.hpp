#ifndef BSPHANDLER_HPP
#define BSPHANDLER_HPP

#include <stdio.h>
#include <vector>
#include <windows.h>

#pragma pack(push)
#pragma pack(1)
struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct Vector
{
	float x;
	float y;
	float z;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dtexinfo_t
{
	float	textureVecs[2][4];	// [s/t][xyz offset]
	float	lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
	int	flags;			// miptex flags	overrides
	int	texdata;		// Pointer to texture name, size, etc.
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dsurfedge_t
{
	signed int edgeID;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dedge_t
{
	unsigned short v[2];	// vertex indices
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dface_t
{
	unsigned short	planenum;		// the plane number
	byte		side;			// faces opposite to the node's plane direction
	byte		onNode;			// 1 of on node, 0 if in leaf
	int		firstedge;		// index into surfedges
	short		numedges;		// number of surfedges
	short		texinfo;		// texture info
	short		dispinfo;		// displacement info
	short		surfaceFogVolumeID;	// ?
	byte		styles[4];		// switchable lighting info
	int		lightofs;		// offset into lightmap lump
	float		area;			// face area in units^2
	int		LightmapTextureMinsInLuxels[2];	// texture lighting info
	int		LightmapTextureSizeInLuxels[2];	// texture lighting info
	int		origFace;		// original face this was split from
	unsigned short	numPrims;		// primitives
	unsigned short	firstPrimID;
	unsigned int	smoothingGroups;	// lightmap smoothing group
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dplane_t
{
	Vector normal;	// normal vector
	float dist;		// distance from origin
	int type;		// plane axis identifier
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct lump_t
{
	int fileofs;	// offset into file (bytes)
	int filelen;	// length of lump
	int version;	// lump format version
	char fourCC[4];	// lump indent code
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dheader_t
{
	int ident;	// BSP file identifier (VBSP or PSBV)
	int version;	// BSP file version 19-21 (21 for CS:GO)
	lump_t lumps[64];	// lump array directory
	int mapRevision;
};
#pragma pack(pop)

class bspHandler {
private:
	dheader_t map_header;
	dface_t* faceArray;
	dsurfedge_t* surfedgeArray;
	dedge_t* edgeArray;
	Vector* vertexArray;
	dtexinfo_t* texinfoArray;
	ColorRGBExp32* lightmapPixelArray;

	int faceArrayLength;
	int surfedgeArrayLength;
	int edgeArrayLength;
	int vertexArrayLength;
	int texinfoArrayLength;
	int lightmapPixelArrayLength;

public:
	int loadBSP(const char *fname);
	void compileTris(std::vector<GLfloat> &triData, std::vector<GLfloat> &uvData, std::vector<GLfloat> &lightmap_uvData);
	GLuint initLightmap();

	void printLump();
	void printVertex();
	void printSurfedge();
	void printEdge();
	void printFace();
	void printTris();
};

#endif