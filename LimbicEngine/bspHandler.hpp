#ifndef BSPHANDLER_HPP
#define BSPHANDLER_HPP

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>

#include "common.hpp"
#include "render.hpp"

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
struct dtexinfo_gld_t
{
	Vector vS;
	float fSShift;    // Texture shift in s direction
	Vector vT;
	float fTShift;    // Texture shift in t direction
	unsigned int iMiptex; // Index into textures array
	unsigned int nFlags;  // Texture flags, seem to always be 0
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
struct dface_gld_t
{
	unsigned short iPlane;          // Plane the face is parallel to
	unsigned short nPlaneSide;      // Set if different normals orientation
	unsigned int iFirstEdge;      // Index of the first surfedge
	unsigned short nEdges;          // Number of consecutive surfedges
	unsigned short iTextureInfo;    // Index of the texture info structure
	byte nStyles[4];       // Specify lighting styles
	unsigned int nLightmapOffset; // Offsets into the raw lightmap data
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
struct lump_gld_t
{
	int nOffset;	// offset into file (bytes)
	int nLength;	// length of lump
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

#pragma pack(push)
#pragma pack(1)
struct dheader_gld_t
{
	int nVersion;	// BSP file version 30 for HL
	lump_gld_t lumps[15];	// lump array directory
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dheader_wad_t
{
	char szMagic[4];	// Should be WAD2/WAD3
	int nDir;			// Number of directory entries
	int nDirOffset;		// Offset into directory
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dentry_wad_t
{
	int nFilePos;		// Offset in WAD
	int nDiskSize;		// Size in file
	int nSize;			// Uncompressed size
	byte nType;			// Type of entry
	bool bCompression;	// 0 if none
	short nDummy;		// Not used
	char szName[16];	// Must be null terminated
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct dmiptex_gld_t
{
	char szName[16];  // Name of texture
	unsigned int nWidth, nHeight;     // Extends of the texture
	unsigned int nOffsets[4]; // Offsets to texture mipmaps BSPMIPTEX;
};
#pragma pack(pop)

typedef int dmiptexoffset_gld_t;

class bspHandler {
private:
	bool gldSrc;
	render* renderHandle;

	dheader_t map_header;
	dface_t* faceArray;
	dsurfedge_t* surfedgeArray;
	dedge_t* edgeArray;
	Vector* vertexArray;
	dtexinfo_t* texinfoArray;
	ColorRGBExp32* lightmapPixelArray;

	dheader_gld_t map_header_gld;
	dface_gld_t* faceArray_gld;
	dtexinfo_gld_t* texinfoArray_gld;
	dmiptexoffset_gld_t* texoffsetArray_gld;
	dmiptex_gld_t* texpointerArray_gld;

	dheader_wad_t tex_header_wad;
	dentry_wad_t* entryArray_wad;


	int faceArrayLength;
	int surfedgeArrayLength;
	int edgeArrayLength;
	int vertexArrayLength;
	int texinfoArrayLength;
	int lightmapPixelArrayLength;
	unsigned int texoffsetLength;
	int entryArrayLength;

	GLuint* gl_maptextures;
	GLuint gl_vao;
	GLuint gl_vertexData;
	GLuint gl_uvData;
	std::vector<GLfloat> gl_vertexBuffer;
	std::vector<GLfloat> gl_uvBuffer;

	std::vector<GLfloat> gl_vertexBuffers[128];
	std::vector<GLfloat> gl_uvBuffers[128];

public:
	int loadBSP(const char *fname, bool isGldSrc);
	void compileTris(std::vector<GLfloat> &triData, std::vector<GLfloat> &uvData, std::vector<GLfloat> &lightmap_uvData);
	void compileBuffers();
	void renderMap();
	GLuint initLightmap();

	void printLump();
	void printVertex();
	void printSurfedge();
	void printEdge();
	void printFace();
	void printTris();
	void printTexPointers();
	void printTexInfo();

	void setRenderPointer(render* renderPointer);
};

#endif