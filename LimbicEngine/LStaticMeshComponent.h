#pragma once

#include "LWorldComponent.h"

class LStaticMeshComponent : public LWorldComponent
{
public:
	LStaticMeshComponent();

	void SetStaticMesh(std::string& filename, std::string& nodename);

	void SetMaterial(std::string& baseColorFilename, std::string& normalFilename, std::string& propertiesFilename);

	void GetDrawInfo(RStaticMesh& mesh, RMaterial& material, mat4& transform);

protected:
	RStaticMesh staticMeshID;
	RMaterial materialID;
};
