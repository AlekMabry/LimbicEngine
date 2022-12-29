#pragma once

#include "LimbicTypes.h"
#include "EEntity.h"

class EStaticWorldGeometry : public EEntity
{
public:
	EStaticWorldGeometry();

	void OnInit(ResourceSystem* hResource) override;

	void OnTick(float dt) override;

	void OnDraw(RenderSystem* hRender) override;

	void GetPropertyInfo(SPropertyInfo*& propertyInfo, uint32& propertyCount) const;

	void GetPropertyHandles(void**& handles);

	std::string staticMeshFilename;
	std::string staticMeshNode;
	std::string baseColorFilename;
	std::string normalFilename;
	std::string propertiesFilename;
	vec3 position;

protected:
	RStaticMesh trussMesh;
	RMaterial trussMaterial;
	mat4 transform;
	float angle;
};
