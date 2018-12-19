#pragma once
#include <fstream>
#include <stdint.h>

#define MAXTEXTURENAME 16
#define MIPLEVELS 4

struct t_wad_header
{
	char szMagic[4];    // should be WAD2/WAD3
	int32_t nDir;       // number of directory entries
	int32_t nDirOffset; // offset into directory
};

struct t_wad_entry
{
	int32_t nFilePos;            // offset in WAD
	int32_t nDiskSize;           // size in file
	int32_t nSize;               // uncompressed size
	int8_t nType;                // type of entry
	bool bCompression;           // 0 if none
	int16_t nDummy;              // not used
	char szName[MAXTEXTURENAME]; // must be null terminated
};

struct t_wad_tex
{
	char szName[MAXTEXTURENAME];  // Name of texture
	uint32_t nWidth, nHeight;     // Extends of the texture
	uint32_t nOffsets[MIPLEVELS]; // Offsets to texture mipmaps BSPMIPTEX;
};

class wadHandler
{
private:
	FILE* wadFile;
public:
	int openWad(const char *fname);
};
