#include <System/ResourceSystem.h>

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
	uint32 dfdByteOffset;	 // Offset to data format descriptor.
	uint32 dfdByteLength;	 // Size of data format descriptor.
	uint32 kvdByteOffset;	 // Offset to key/value pairs.
	uint32 kvdByteLength;	 // Size of key/value pairs (including padding).
	uint32 sgdByteOffset;	 // Offset to super compression global data.
	uint32 sgdByteLength;	 // Size of super compression global data.
};

struct SKTX2LevelIndex
{
	uint64 byteOffset;
	uint64 byteLength;
	uint64 uncompressedByteLength;
};

ResourceSystem::ResourceSystem()
{
	fbxManager = FbxManager::Create();
	fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxIOSettings);
}

ResourceSystem::~ResourceSystem()
{
	fbxIOSettings->Destroy();
	fbxManager->Destroy();
}

RStaticMesh ResourceSystem::RequestStaticMesh(std::string& filename, std::string& nodeName)
{
	std::string meshName = filename + nodeName;
	if (resourceLookup.count(meshName) == 0)
	{
		SMesh* mesh = LoadMesh(filename, nodeName);
		RStaticMesh hMesh;
		SStaticVertex* vertexBuffer;
		uint32* indexBuffer;
		//renderSystem->CreateStaticMesh(static_cast<uint32>(mesh->vertices.size()), static_cast<uint32>(mesh->indices.size()), hMesh, vertexBuffer, indexBuffer);
		//memcpy(vertexBuffer, mesh->vertices.data(), mesh->vertices.size() * sizeof(SStaticVertex));
		//memcpy(indexBuffer, mesh->indices.data(), mesh->indices.size() * sizeof(uint32));
		//delete mesh;
		//renderSystem->SubmitAssets();
		//resourceLookup.insert(std::make_pair(meshName, hMesh));
		return hMesh;
	}
	else
	{
		return resourceLookup[meshName];
	}
}

RTexture ResourceSystem::RequestTexture(std::string& filename)
{
	if (resourceLookup.count(filename) == 0)
	{
		RTexture hTexture;
		void* textureBuffer;
		renderSystem->CreateTexture(1024, 1024, eTextureFormatDXT1, hTexture, textureBuffer);
		LoadTextureKTX2(filename, textureBuffer);
		renderSystem->SubmitAssets();
		resourceLookup.insert(std::make_pair(filename, hTexture));
		return hTexture;
	}
	else
	{
		return resourceLookup[filename];
	}
}

RMaterial ResourceSystem::RequestMaterial(std::string& baseColorFilename, std::string& normalFilename, std::string& propertiesFilename)
{
	//RTexture baseColor = RequestTexture(baseColorFilename);
	//RTexture normal = RequestTexture(normalFilename);
	//RTexture properties = RequestTexture(propertiesFilename);

	RMaterial material;
	//renderSystem->CreateMaterial(baseColor, normal, properties, material);
	return material;
}

FbxNode* ResourceSystem::GetFbxNode(FbxNode* root, std::string& nodeName) const
{
	if (root)
	{
		if (nodeName == root->GetName())
			return root;
		else
		{
			for (int i = 0; i < root->GetChildCount(); i++)
			{
				FbxNode* node = GetFbxNode(root->GetChild(i), nodeName);
				if (node)
					return node;
			}
		}
	}
	return nullptr;
}

void ResourceSystem::PrintNode(FbxNode* node, int indent)
{
	for (int i = 0; i < indent; i++)
		std::cout << "\t";

	std::cout << node->GetName() << std::endl;

	for (int k = 0; node->GetNodeAttributeCount(); k++)
	{
		FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(k);
		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
			std::cout << "Has mesh\n";
	}

	for (int j = 0; j < node->GetChildCount(); j++)
		PrintNode(node->GetChild(j), indent + 1);
}

SMesh* ResourceSystem::LoadMesh(std::string& filename, std::string& nodeName)
{
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (!fbxImporter->Initialize(filename.c_str(), -1, fbxManager->GetIOSettings()))
	{
		std::cerr << "[WARNING] Unable to load " << filename << std::endl;
		return nullptr;
	}

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	FbxNode* fbxMeshNode = GetFbxNode(fbxRootNode, nodeName);

	if (!fbxMeshNode)
	{
		std::cerr << "[WARNING] Unable to find " << nodeName << " in " << filename << std::endl;
		return nullptr;
	}

	if (!fbxMeshNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		std::cerr << "[WARNING] " << nodeName << " node does not contain a mesh attribute" << std::endl;
		return nullptr;
	}

	SMesh* outputMesh = new (SMesh);

	FbxMesh* fbxMesh = fbxMeshNode->GetMesh();
	uint32 fbxMeshPolygonCount = fbxMesh->GetPolygonCount();
	uint32 fbxMeshControlPointCount = fbxMesh->GetControlPointsCount();
	uint32 fbxMeshUVCount = fbxMesh->GetElementUVCount();
	FbxGeometryElementUV* fbxMeshUV = fbxMesh->GetElementUV(0);

	uint32 currentMeshIndex = 0;

	for (uint32 i = 0; i < fbxMeshPolygonCount; i++)
	{
		uint32 faceVertexCount = fbxMesh->GetPolygonSize(i);

		/* For every control point (vertex) referenced by this polygon, bake it into an SStaticVertex and add it
		to the vertices vector. */
		for (uint32 j = 0; j < faceVertexCount; j++)
		{
			uint32 fbxControlPointIndex = fbxMesh->GetPolygonVertex(i, j);
			uint32 fbxUVIndex = fbxMesh->GetTextureUVIndex(i, j);
			FbxVector4 fbxControlPoint = fbxMesh->GetControlPointAt(fbxControlPointIndex);
			FbxVector2 fbxUV = fbxMeshUV->GetDirectArray().GetAt(fbxUVIndex);
			SStaticVertex vertex{};
			for (uint32 k = 0; k < 3; k++)
			{
				vertex.position[k] = static_cast<float>(fbxControlPoint.mData[k]);	  // Double to float
			}
			for (uint32 k = 0; k < 2; k++)
			{
				vertex.textureUV[k] = static_cast<float>(fbxUV.mData[k]);
			}

			outputMesh->vertices.push_back(vertex);
		}

		/* Generate triangle indices from n-gon. Creates triangle fan from first vertex. */
		for (int j = 1; j < faceVertexCount - 1; j++)
		{
			outputMesh->indices.push_back(currentMeshIndex);
			outputMesh->indices.push_back(currentMeshIndex + j);
			outputMesh->indices.push_back(currentMeshIndex + j + 1);
		}

		currentMeshIndex += faceVertexCount;
	}

	// meshes.insert(std::pair<std::string, SMesh>(nodeName, outputMesh));
	return outputMesh;
}

void ResourceSystem::LoadTextureKTX2(std::string& filename, void* buffer)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");

	SKTX2Header header;
	SKTX2Index index;
	fread_s(&header, sizeof(SKTX2Header), sizeof(SKTX2Header), 1, file);

	if (strncmp(header.identifier, ktx2FileIdentifier, 12))
	{
		std::cout << "[WARNING] " << filename << " is not a valid KTX2 file!" << std::endl;
	}

	fread_s(&index, sizeof(SKTX2Index), sizeof(SKTX2Index), 1, file);

	// Levels array starts with the base mip level (largest image)
	std::vector<SKTX2LevelIndex> levels(header.levelCount);
	fseek(file, 8, SEEK_CUR);
	fread_s(levels.data(), header.levelCount * sizeof(SKTX2LevelIndex), sizeof(SKTX2LevelIndex), header.levelCount, file);

	// Load first mip level
	fseek(file, levels[0].byteOffset, SEEK_SET);
	fread(buffer, 8, (header.pixelWidth * header.pixelHeight) / 16, file);

	fclose(file);
}

//---- Vulkan GPU memory management. ----
uint32 ResourceSystem::CreateDeviceMemoryBlock(EMemoryBlockUsage usage)
{
	SMemoryBlock deviceBlock{};
	deviceBlock.usage = usage;
	switch (usage)
	{
	case eMemoryBlockUsageGeometry:
		CreateBuffer(MEMORY_BLOCK_SIZE, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceBlock.buffer,
			deviceBlock.memory);
		break;
	case eMemoryBlockUsageUniforms:
		CreateBuffer(MEMORY_BLOCK_SIZE, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceBlock.buffer,
			deviceBlock.memory);
		break;
	case eMemoryBlockUsageImages:
		AllocateDeviceMemory(MEMORY_BLOCK_SIZE, memoryBlockUsageLocation[usage], deviceBlock.memory);
		break;
	case eMemoryBlockUsageStaging:
		break;
	}
	deviceMemory.push_back(deviceBlock);
	return static_cast<uint32>(deviceMemory.size() - 1);
}