#include "EntityComponent.h"

EComponentType EntityComponent::GetType() const
{
	return ComponentType_EntityComponent;
}

void EntityComponent::SetTickEnabled(bool bTickEnabled)
{
	this->bTickEnabled = bTickEnabled;
}

bool EntityComponent::IsTickEnabled() const
{
	return bTickEnabled;
}

void EntityComponent::OnTick(uint32 dt)
{

}