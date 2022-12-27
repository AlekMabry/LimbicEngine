#pragma once

#include "EEntity.h"
#include "StaticMeshComponent.h"

class EStaticWorldGeometry : public EEntity
{
public:
	EStaticWorldGeometry();

protected:
	std::unique_ptr<StaticMeshComponent> trussMesh;
};
