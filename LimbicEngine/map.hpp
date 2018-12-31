#pragma once
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <iostream>

#include "common.hpp"
#include "render.hpp"

#define LUMP_ENTITIES      0
#define LUMP_PLANES        1
#define LUMP_TEXTURES      2
#define LUMP_VERTICES      3
#define LUMP_VISIBILITY    4
#define LUMP_NODES         5
#define LUMP_TEXINFO       6
#define LUMP_FACES         7
#define LUMP_LIGHTING      8
#define LUMP_CLIPNODES     9
#define LUMP_LEAVES       10
#define LUMP_MARKSURFACES 11
#define LUMP_EDGES        12
#define LUMP_SURFEDGES    13
#define LUMP_MODELS       14
#define HEADER_LUMPS      15

#define CONTENTS_EMPTY        -1
#define CONTENTS_SOLID        -2
#define CONTENTS_WATER        -3
#define CONTENTS_SLIME        -4
#define CONTENTS_LAVA         -5
#define CONTENTS_SKY          -6
#define CONTENTS_ORIGIN       -7
#define CONTENTS_CLIP         -8
#define CONTENTS_CURRENT_0    -9
#define CONTENTS_CURRENT_90   -10
#define CONTENTS_CURRENT_180  -11
#define CONTENTS_CURRENT_270  -12
#define CONTENTS_CURRENT_UP   -13
#define CONTENTS_CURRENT_DOWN -14
#define CONTENTS_TRANSLUCENT  -15

#define MAX_MAP_HULLS 4

#pragma pack(push)
#pragma pack(1)

struct dbsplump_t
{
	int nOffset; // File offset to data
	int nLength; // Length of data
};

struct dbspheader_t
{
	int nVersion;           // Must be 30 for a valid HL BSP file
	dbsplump_t lump[HEADER_LUMPS]; // Stores the directory of lumps
};

struct dwadheader_t
{
	char szMagic[4];	// Should be WAD2/WAD3
	int nDir;			// Number of directory entries
	int nDirOffset;		// Offset into directory
};

struct dvec3_t
{
	float x;
	float y;
	float z;
};

typedef const char dentity_t;

struct dplane_t
{
	dvec3_t vNorm;	// Normal vector
	float vDist;	// Distance from origin
	int iType;		// Plane axis identifier
};

typedef unsigned int dtexheader_t;

typedef int dtexoffset_t;

typedef dvec3_t dvertex_t;

// VIS Lump

struct dnode_t
{
	unsigned int iPlane;            // Index into Planes lump
	short iChildren[2];       // If > 0, then indices into Nodes // otherwise bitwise inverse indices into Leafs
	short nMins[3], nMaxs[3]; // Defines bounding box
	unsigned short firstFace, nFaces; // Index and count into Faces
};

struct dtexinfo_t
{
	dvec3_t vS;				// Multiply vertex position by this to get U (in pixels)
	float fSShift;			// Texture shift in s direction
	dvec3_t vT;				// Multiply vertex position by this to get V (in pixels)
	float fTShift;			// Texture shift in t direction
	unsigned int iMiptex;	// Index into textures array
	unsigned int nFlags;	// Texture flags, seem to always be 0
};

struct dface_t
{
	unsigned short iPlane;			// Plane the face is parallel to
	unsigned short nPlaneSide;		// Set if different normals orientation
	unsigned int iFirstEdge;		// Index of the first surfedge
	unsigned short nEdges;			// Number of consecutive surfedges
	unsigned short iTextureInfo;	// Index of the texture info structure
	byte nStyles[4];				// Specify lighting styles
	unsigned int nLightmapOffset;	// Offsets into the raw lightmap data
};

struct d24color_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

typedef d24color_t dlightmap_t;

struct dclipnode_t
{
	int iPlane;       // Index into planes
	short iChildren[2]; // negative numbers are contents
};

struct dleave_t
{
	int nContents;                         // Contents enumeration
	int nVisOffset;                        // Offset into the visibility lump
	short nMins[3], nMaxs[3];                // Defines bounding box
	unsigned short iFirstMarkSurface, nMarkSurfaces; // Index and count into marksurfaces array
	unsigned char nAmbientLevels[4];                 // Ambient sound levels
};

typedef unsigned short dmarksurface_t;

struct dedge_t
{
	unsigned short iV[2];	// vertex indices
};

struct dsurfedge_t
{
	int iEdgeID;		// Index into dedge_t array	
};

typedef struct dmodel_t
{
	float nMins[3], nMaxs[3];          // Defines bounding box
	dvec3_t vOrigin;                  // Coordinates to move the // coordinate system
	int iHeadnodes[MAX_MAP_HULLS]; // Index into nodes array
	int nVisLeafs;                 // ???
	int iFirstFace, nFaces;        // Index and count into faces
};


struct dtexdata_t
{
	char szName[16];  // Name of texture
	unsigned int nWidth, nHeight;     // Extends of the texture
	unsigned int nOffsets[4]; // Offsets to texture mipmaps BSPMIPTEX;
};

struct dentry_t
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

class map
{
private:
	
public:
	// BSP File Vars
	dbspheader_t bspHeader;
	dplane_t* planeArray;
	int planeArrayLen;
	// LUMP_TEXTURES
	dvertex_t* vertexArray;
	int vertexArrayLen;
	// Vis leafs
	dnode_t* nodeArray;
	int nodeArrayLen;
	dtexinfo_t* texinfoArray;
	int texinfoArrayLen;
	dface_t* faceArray;
	int faceArrayLen;
	// Lightmap
	dclipnode_t* clipnodeArray;
	int clipnodeArrayLen;
	dleave_t* leafArray;
	int leafArrayLen;
	dmarksurface_t* marksurfaceArray;
	int marksurfaceArrayLen;
	dedge_t* edgeArray;
	int edgeArrayLen;
	dsurfedge_t* surfedgeArray;
	int surfedgeArrayLen;
	dmodel_t* modelArray;
	int modelArrayLen;
	dtexdata_t* texdataArray;
	int texdataArrayLen;
	dtexheader_t texHeader;
	dtexoffset_t* texoffsetArray;

	// WAD File Vars
	dwadheader_t wadHeader;
	dentry_t* wadEntryArray;
	int wadEntryArrayLen;

	// OpenGL Stuffs
	GLuint* gl_maptextures;
	GLuint gl_vao;
	GLuint gl_vertexData;
	GLuint gl_uvData;
	std::vector<GLfloat> gl_vertexBuffers[128];
	std::vector<GLfloat> gl_uvBuffers[128];

	// Handles
	render* renderHandle;

	int load(const char *bspname, const char *wadname);
	void compileBuffers();
	void renderMap();

	void setRenderHandle(render* handle);
};