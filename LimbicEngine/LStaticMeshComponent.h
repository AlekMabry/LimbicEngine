#pragma once

#include "LWorldComponent.h"

#include <string>

class LStaticMeshComponent : public LWorldComponent
{
public:
	LStaticMeshComponent();

	void SetStaticMesh(const std::string& filename, const std::string& nodename);

	void SetMaterial(const std::string& baseColorFilename, const std::string& normalFilename, const std::string& propertiesFilename);

	void SetResources(RStaticMesh staticMeshID, RMaterial materialID);

	void GetResourceInfo(
		std::string& mesh, std::string& node, std::string& baseColor, std::string& normal, std::string& properties);

	void GetDrawInfo(RStaticMesh& mesh, RMaterial& material);

protected:
	std::string meshFilename;
	std::string nodeName;
	std::string baseColorFilename;
	std::string normalFilename;
	std::string propertiesFilename;
	RStaticMesh staticMeshID;
	RMaterial materialID;
};
