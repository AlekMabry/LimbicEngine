#pragma once

#include "EEntity.h"
#include "EStaticWorldGeometry.h"

#include <functional>

#define LSTRINGIFY(s) #s

#define LREGISTER(entityClass) \
	NewEntity.insert(std::make_pair(std::string(LSTRINGIFY(entityClass)), [this]() -> EEntity* { return SpawnEntityCast<entityClass>(); }));

class WorldSystem
{
public:
	WorldSystem();

	void LoadFromJSON(const char* filename);

	void GetEntities(EEntity**& entities, uint32& entityCount);

	template <typename TEntityClass>
	TEntityClass* SpawnEntity(std::string name, uint32& entityId)
	{
		TEntityClass* entity = new TEntityClass();
		entities.push_back(static_cast<EEntity*>(entity));
		return entity;
	}

	template <typename TEntityClass>
	EEntity* SpawnEntityCast()
	{
		TEntityClass* entity = new TEntityClass();
		entities.push_back(static_cast<EEntity*>(entity));
		return static_cast<EEntity*>(entity);
	}

protected:
	std::vector<EEntity*> entities;
	std::unordered_map<std::string, std::function<EEntity*()>> NewEntity;
};