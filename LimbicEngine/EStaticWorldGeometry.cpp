#include "EStaticWorldGeometry.h"
#include <glm/gtc/matrix_transform.hpp>

EStaticWorldGeometry::EStaticWorldGeometry() : EEntity()
{
	enabledEventFlags = ENTITY_EVENT_ENABLED_TICK | ENTITY_EVENT_ENABLED_DRAW;
}

void EStaticWorldGeometry::OnInit(ResourceSystem* hResource)
{
	trussMesh = hResource->RequestStaticMesh(staticMeshFilename, staticMeshNode);
	trussMaterial = hResource->RequestMaterial(baseColorFilename, normalFilename, propertiesFilename);

	transform = glm::rotate(mat4(1.0f), glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
	transform = glm::translate(transform, position);
}

void EStaticWorldGeometry::OnTick(float dt)
{
}

void EStaticWorldGeometry::OnDraw(RenderSystem* hRender)
{
	hRender->DrawStaticMesh(trussMesh, trussMaterial, transform);
}

void EStaticWorldGeometry::GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount)
{
	LPROPERTY(propertyInfo[0], "staticMeshFilename", "Static Mesh Filename", ePropertyTypeFilename, staticMeshFilename)
	LPROPERTY(propertyInfo[1], "staticMeshNode", "Mesh Node Name", ePropertyTypeFilename, staticMeshNode)
	LPROPERTY(propertyInfo[2], "baseColorFilename", "Base Color Texture Filename", ePropertyTypeFilename, baseColorFilename)
	LPROPERTY(propertyInfo[3], "normalFilename", "Normal Texture Filename", ePropertyTypeFilename, normalFilename)
	LPROPERTY(propertyInfo[4], "propertiesFilename", "Properties Texture Filename", ePropertyTypeFilename, propertiesFilename)
	LPROPERTY(propertyInfo[5], "position", "Position", ePropertyTypeVec3, position);
	LPROPERTY(propertyInfo[6], "rotation", "Rotation", ePropertyTypeVec3, rotation);

	propertyCount = 7;
}