#include "LStaticMeshComponent.h"

LStaticMeshComponent::LStaticMeshComponent() : LWorldComponent()
{
	bRenderEnabled = true;
	bEditorRenderEnabled = false;
	bCollisionEnabled = false;
}

void LStaticMeshComponent::SetStaticMesh(const std::string& filename, const std::string& nodename)
{
	this->meshFilename = filename;
	this->nodeName = nodename;
}

void LStaticMeshComponent::SetMaterial(
	const std::string& baseColorFilename, const std::string& normalFilename, const std::string& propertiesFilename)
{
	this->baseColorFilename = baseColorFilename;
	this->normalFilename = normalFilename;
	this->propertiesFilename = propertiesFilename;
}

void LStaticMeshComponent::SetResources(RStaticMesh staticMeshID, RMaterial materialID)
{
	this->staticMeshID = staticMeshID;
	this->materialID = materialID;
}

void LStaticMeshComponent::GetResourceInfo(
	std::string& mesh, std::string& node, std::string& baseColor, std::string& normal, std::string& properties)
{
	mesh = this->meshFilename;
	node = this->nodeName;
	baseColor = this->baseColorFilename;
	normal = this->normalFilename;
	properties = this->propertiesFilename;
}

void LStaticMeshComponent::GetDrawInfo(RStaticMesh& mesh, RMaterial& material)
{
	mesh = this->staticMeshID;
	material = this->materialID;
}