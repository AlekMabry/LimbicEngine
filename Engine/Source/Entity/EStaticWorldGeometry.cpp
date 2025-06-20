#include <Entity/EStaticWorldGeometry.h>
#include <System/RenderSystem.h>
#include <System/ResourceSystem.h>
#include <glm/gtc/matrix_transform.hpp>

EStaticWorldGeometry::EStaticWorldGeometry(Game& game)
	: EEntity(game)
{
	enabledEventFlags = ENTITY_EVENT_ENABLED_TICK | ENTITY_EVENT_ENABLED_DRAW;
}

void EStaticWorldGeometry::OnInit()
{
	trussMesh = pResource->RequestStaticMesh(staticMeshFilename, staticMeshNode);
	trussMaterial = pResource->RequestMaterial(baseColorFilename, normalFilename, propertiesFilename);
}

void EStaticWorldGeometry::OnTick(float)
{
}

void EStaticWorldGeometry::OnDraw()
{
	transform = glm::rotate(mat4(1.0f), glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
	transform = glm::translate(transform, position);
	pRender->DrawStaticMesh(trussMesh, trussMaterial, transform, selected);
}

std::map<std::string, SPropertyInfo> EStaticWorldGeometry::GetPropertyInfo()
{
	std::map<std::string, SPropertyInfo> properties;
	LPROPERTY(staticMeshFilename, "Static Mesh Filename", ePropertyTypeFilename)
	LPROPERTY(staticMeshNode, "Mesh Node Name", ePropertyTypeFilename)
	LPROPERTY(baseColorFilename, "Base Color Texture Filename", ePropertyTypeFilename)
	LPROPERTY(normalFilename, "Normal Texture Filename", ePropertyTypeFilename)
	LPROPERTY(propertiesFilename, "Properties Texture Filename", ePropertyTypeFilename)
	LPROPERTY(position, "Position", ePropertyTypeVec3)
	LPROPERTY(rotation, "Rotation", ePropertyTypeVec3)
	LPROPERTY(selected, "Selected", ePropertyTypeBool)
	return properties;
}