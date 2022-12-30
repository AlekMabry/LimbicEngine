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
	angle = 0.0f;
}

void EStaticWorldGeometry::OnTick(float dt)
{
	angle += dt * glm::radians(15.0f);
	if (angle > glm::radians(360.0f))
	{
		angle -= glm::radians(360.0f);
	}
}

void EStaticWorldGeometry::OnDraw(RenderSystem* hRender)
{
	transform = glm::rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, position);

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

	propertyCount = 6;
}