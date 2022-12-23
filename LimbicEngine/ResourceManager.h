#pragma once

#include "LimbicTypes.h"

#include <fbxsdk.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "LimbicTypes.h"

const char ktx2FileIdentifier[12] = {0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32, 0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};

struct SKTX2Header
{
	char identifier[12];
	uint32 vkFormat;
	uint32 typeSize;
	uint32 pixelWidth;
	uint32 pixelHeight;
	uint32 pixelDepth;
	uint32 layerCount;
	uint32 faceCount;
	uint32 levelCount;
	uint32 supercompressionScheme;
};

struct SKTX2Index
{
	uint32 dfdByteOffset;	// Offset to data format descriptor.
	uint32 dfdByteLength;	// Size of data format descriptor.
	uint32 kvdByteOffset;	// Offset to key/value pairs.
	uint32 kvdByteLength;	// Size of key/value pairs (including padding).
	uint32 sgdByteOffset;	// Offset to super compression global data.
	uint32 sgdByteLength;	// Size of super compression global data.
};

struct SKTX2LevelIndex
{
	uint64 byteOffset;
	uint64 byteLength;
	uint64 uncompressedByteLength;
};

class ResourceManager
{
public:
	ResourceManager();
	void PrintNode(FbxNode* node, int indent = 0);
	SMesh* LoadMesh(std::string &filename, std::string &nodeName);
	void LoadTextureKTX2(std::string& filename, void* buffer);
	~ResourceManager();

protected:
	/** Recursive search by node name. */
	FbxNode* GetFbxNode(FbxNode *root, std::string &nodeName) const;

	FbxManager* fbxManager;
	FbxIOSettings* fbxIOSettings;
	std::unordered_map<std::string, SMesh> meshes;
};
