#include "WorldSystem.h"

void WorldSystem::GetEntities(EEntity**& entities, uint32& entityCount)
{
	entities = this->entities.data();
	entityCount = static_cast<uint32>(this->entities.size());
}