#include "EStaticWorldGeometry.h"

EStaticWorldGeometry::EStaticWorldGeometry()
{
	worldMesh = new LStaticMeshComponent();
	root = worldMesh;
	std::string mesh = "C:/Users/alekm/Desktop/Assets/Static/Truss/Truss.fbx";
	std::string truss = "Truss";
	worldMesh->SetStaticMesh(mesh, truss);
	std::string baseColor = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_BaseColor.KTX2";
	std::string normal = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_BaseColor.KTX2";
	std::string properties = "C:/Users/alekm/Desktop/Outpost731/Textures/Truss_BaseColor.KTX2";
	worldMesh->SetMaterial(baseColor, normal, properties);
}