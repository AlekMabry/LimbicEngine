#pragma once

#include "EEntity.h"
#include "WorldComponent.h"
#include "glm/vec4.hpp"

class World
{
public:
	~World();

	template <class EntityClass>
	EntityClass* SpawnPhysicalEntity(EntityClass type, vec4 position, vec4 rotation);

	void CleanPendingKills();

	std::vector<EEntity*>* GetEntities();

protected:
	std::vector<EEntity*> entities;
};