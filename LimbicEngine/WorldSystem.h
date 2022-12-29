#pragma once

#include "EEntity.h"
#include "EStaticWorldGeometry.h"

class WorldSystem
{
public:
	void GetEntities(EEntity**& entities, uint32& entityCount);

	template <typename TEntityClass>
	TEntityClass* SpawnEntity(std::string name, uint32& entityId)
	{
		TEntityClass* entity = new TEntityClass();
		entities.push_back(static_cast<EEntity*>(entity));
		return entity;
	}

protected:
	std::vector<EEntity*> entities;
};