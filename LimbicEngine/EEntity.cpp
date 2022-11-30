#include "EEntity.h"

EEntity::EEntity()
{
	rootComponent = std::make_unique<EntityComponent>();
}

EntityComponent* EEntity::GetRootComponent() const
{
	return rootComponent.get();
}