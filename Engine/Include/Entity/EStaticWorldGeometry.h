#pragma once

#include <LimbicTypes.h>
#include <Entity/EEntity.h>

class EStaticWorldGeometry : public EEntity
{
public:
	EStaticWorldGeometry(Game& game);

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	std::map<std::string, SPropertyInfo> GetPropertyInfo() override;

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
