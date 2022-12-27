#pragma once

#include "EEntity.h"
#include "LStaticMeshComponent.h"

class EStaticWorldGeometry : public EEntity
{
public:
	EStaticWorldGeometry();

protected:
	LStaticMeshComponent* worldMesh;
};
