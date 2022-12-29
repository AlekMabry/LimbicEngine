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

void EStaticWorldGeometry::GetPropertyInfo(SPropertyInfo*& propertyInfo, uint32& propertyCount) const
{
	propertyCount = 6;

	propertyInfo[0].label = std::string("Static Mesh Filename");
	propertyInfo[0].name = std::string("staticMeshFilename");
	propertyInfo[0].type = ePropertyTypeFilename;

	propertyInfo[1].label = std::string("Mesh Node Name");
	propertyInfo[1].name = std::string("staticMeshNode");
	propertyInfo[1].type = ePropertyTypeFilename;
	
	propertyInfo[2].label = std::string("Base Color Texture Filename");
	propertyInfo[2].name = std::string("baseColorFilename");
	propertyInfo[2].type = ePropertyTypeFilename;

	propertyInfo[3].label = std::string("Normal Texture Filename");
	propertyInfo[3].name = std::string("normalFilename");
	propertyInfo[3].type = ePropertyTypeFilename;

	propertyInfo[4].label = std::string("Properties Texture Filename");
	propertyInfo[4].name = std::string("propertiesFilename");
	propertyInfo[4].type = ePropertyTypeFilename;

	propertyInfo[5].label = std::string("Position");
	propertyInfo[5].name = std::string("position");
	propertyInfo[5].type = ePropertyTypeVec3;
}


void EStaticWorldGeometry::GetPropertyHandles(void**& handles)
{
	handles[0] = static_cast<void*>(&staticMeshFilename);
	handles[1] = static_cast<void*>(&staticMeshNode);
	handles[2] = static_cast<void*>(&baseColorFilename);
	handles[3] = static_cast<void*>(&normalFilename);
	handles[4] = static_cast<void*>(&propertiesFilename);
	handles[5] = static_cast<void*>(&position);
}