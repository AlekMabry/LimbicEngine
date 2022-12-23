#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	fbxManager = FbxManager::Create();
	fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxIOSettings);
}

ResourceManager::~ResourceManager()
{
	fbxIOSettings->Destroy();
	fbxManager->Destroy();
}

void ResourceManager::PrintNode(FbxNode* node, int indent)
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

SMesh* ResourceManager::LoadMesh(std::string& filename, std::string& nodeName)
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

	SMesh* outputMesh = new(SMesh);

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

	//meshes.insert(std::pair<std::string, SMesh>(nodeName, outputMesh));
	return outputMesh;
}

void ResourceManager::LoadTextureKTX2(std::string& filename, void* buffer)
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
	fread_s(levels.data(), header.levelCount * sizeof(SKTX2LevelIndex), sizeof(SKTX2LevelIndex), header.levelCount, file);

	// Load first mip level
	fseek(file, levels[0].byteOffset + (27 * 8), SEEK_SET);
	fread(buffer, 8, (header.pixelWidth * header.pixelHeight) / 16, file);

	fclose(file);
}

FbxNode* ResourceManager::GetFbxNode(FbxNode* root, std::string& nodeName) const
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