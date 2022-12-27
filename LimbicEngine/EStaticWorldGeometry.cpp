#include "EStaticWorldGeometry.h"

EStaticWorldGeometry::EStaticWorldGeometry()
{
	bTickEnabled = false;
	trussMesh = std::make_unique<StaticMeshComponent>();
}

