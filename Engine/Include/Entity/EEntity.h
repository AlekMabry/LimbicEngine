#pragma once

#include <LimbicProperties.h>

#include <map>

#define ENTITY_EVENT_ENABLED_TICK	1
#define ENTITY_EVENT_ENABLED_DRAW	2

class Game;
class ResourceSystem;
class RenderSystem;
class WorldSystem;

class EEntity
{
public:
	EEntity(Game& game);
	~EEntity() = default;

	/** Executed after properties are set by factory or creation entity. */
	virtual void OnInit() = 0;

	/** Executed every tick. 
		@param[in]	dt	Delta time since last tick (seconds). */
	virtual void OnTick(float dt) = 0;

	/** Executed before frame is drawn. Used to submit draw instructions to renderer. */
	virtual void OnDraw() = 0;

	/**	Returns initialization properties available in the map file or editor. */
	virtual std::map<std::string, SPropertyInfo> GetPropertyInfo() = 0;

	uint32 GetEnabledEventFlags() const;

	bool IsPendingKill() const;

protected:
	uint32 enabledEventFlags;
	bool bPendingKill;

	Game* pGame;
	ResourceSystem* pResource;
	RenderSystem* pRender;
	WorldSystem* pWorld;
};
