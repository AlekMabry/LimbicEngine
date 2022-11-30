#pragma once

#include "EntityComponent.h"

class EEntity
{
	EEntity();

	EntityComponent* GetRootComponent() const;

protected:
	bool bIsTickEnabled;
	std::unique_ptr<EntityComponent> rootComponent;
};

