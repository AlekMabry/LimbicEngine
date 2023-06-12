#pragma once

#include <LimbicTypes.h>
#include <Entity/EEntity.h>

class EStaticWorldGeometry : public EEntity
{
public:
	EStaticWorldGeometry();

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	void GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount) override;

	std::string staticMeshFilename;
	std::string staticMeshNode;
	std::string baseColorFilename;
	std::string normalFilename;
	std::string propertiesFilename;
	vec3 position;
	vec3 rotation;

protected:
	RStaticMesh trussMesh;
	RMaterial trussMaterial;
	mat4 transform;
};
