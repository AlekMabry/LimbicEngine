#include "LStaticMeshComponent.h"

LStaticMeshComponent::LStaticMeshComponent() : LWorldComponent()
{
	bRenderEnabled = true;
	bEditorRenderEnabled = false;
	bCollisionEnabled = false;
}

void LStaticMeshComponent::SetStaticMesh(std::string& filename, std::string& nodename)
{
	staticMeshID = GetResourceManager()->RequestStaticMesh(filename)
	this->mesh = mesh;
	this->material = material;
}

void StaticMeshComponent::GetDrawInfo(RStaticMesh& mesh, RMaterial& material, mat4& transform)
{
	mesh = this->mesh;
	material = this->material;
}