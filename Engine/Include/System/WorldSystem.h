#pragma once

#include <LimbicTypes.h>

#include <eventpp/callbacklist.h>

#include <string>
#include <functional>

using namespace eventpp;

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

	std::vector<EEntity*>& GetEntities();

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

	CallbackList<void()> eMapUpdated;

protected:
	std::vector<EEntity*> entities;
	std::unordered_map<std::string, std::function<EEntity*()>> NewEntity;
	Game* pGame;
};