#include "World.h"

World::~World()
{
	for (EEntity* entity : entities)
	{
		delete entity;
	}
}

template <class EntityClass>
EntityClass* World::SpawnPhysicalEntity(EntityClass type, vec4 position, vec4 rotation)
{
	EEntity* entity = new type(this);
	WorldComponent* root = entity->GetRootComponent();
	root->SetPosition(position);
	root->SetRotation(rotation);
	entities.push_back(entity);
}

void World::CleanPendingKills()
{
	size_t cursor = 0;
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (!entities[i]->IsPendingKill())
		{
			entities[cursor] = entities[i];
			cursor = i + 1;
		}
		else
		{
			delete entities[i];
		}
	}
	entities.resize(cursor);
}

std::vector<EEntity*>* World::GetEntities()
{
	return &entities;
}