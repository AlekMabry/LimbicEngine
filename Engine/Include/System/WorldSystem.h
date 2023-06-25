#pragma once

#include <LimbicTypes.h>

#include <string>
#include <functional>

#define LREGISTER(entityClass) \
	NewEntity.insert(std::make_pair(std::string(LSTRINGIFY(entityClass)), [this]() -> EEntity* { return SpawnEntityCast<entityClass>(); }));

class Game;
class EEntity;

class WorldSystem
{
public:
	WorldSystem(Game& game);
	~WorldSystem();

	void OnTick(float dt);

	void OnDraw();

	void LoadFromJSON(const char* filename);

	void GetEntities(EEntity**& entities, uint32& entityCount);

	template <typename TEntityClass>
	TEntityClass* SpawnEntity()
	{
		TEntityClass* entity = new TEntityClass(*pGame);
		entities.push_back(static_cast<EEntity*>(entity));
		return entity;
	}

	template <typename TEntityClass>
	EEntity* SpawnEntityCast()
	{
		TEntityClass* entity = SpawnEntity<TEntityClass>();
		return static_cast<EEntity*>(entity);
	} 


protected:
	std::vector<EEntity*> entities;
	std::unordered_map<std::string, std::function<EEntity*()>> NewEntity;
	Game* pGame;
};