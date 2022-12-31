#pragma once

#include "LimbicTypes.h"
#include "EEntity.h"
#include "ResourceSystem.h"
#include "RenderSystem.h"
#include "IOSystem.h"

#include <functional>

#define LREGISTER(entityClass) \
	NewEntity.insert(std::make_pair(std::string(LSTRINGIFY(entityClass)), [this]() -> EEntity* { return SpawnEntityCast<entityClass>(); }));

class WorldSystem
{
public:
	WorldSystem();

	void LoadFromJSON(const char* filename);

	void GetEntities(EEntity**& entities, uint32& entityCount);

	template <typename TEntityClass>
	TEntityClass* SpawnEntity()
	{
		TEntityClass* entity = new TEntityClass();
		entities.push_back(static_cast<EEntity*>(entity));
		entity->hIO = hIO;
		entity->hResource = hResource;
		entity->hRender = hRender;
		return entity;
	}

	template <typename TEntityClass>
	EEntity* SpawnEntityCast()
	{
		TEntityClass* entity = SpawnEntity<TEntityClass>();
		return static_cast<EEntity*>(entity);
	}

	IOSystem* hIO;
	RenderSystem* hRender;
	ResourceSystem* hResource;

protected:
	std::vector<EEntity*> entities;
	std::unordered_map<std::string, std::function<EEntity*()>> NewEntity;
};