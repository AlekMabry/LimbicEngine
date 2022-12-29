#pragma once

#include "ResourceSystem.h"
#include "RenderSystem.h"
#include "LimbicProperties.h"

#define ENTITY_EVENT_ENABLED_TICK	1
#define ENTITY_EVENT_ENABLED_DRAW	2

class EEntity
{
public:
	EEntity();

	/** Executed after properties are set by factory or creation entity. */
	virtual void OnInit(ResourceSystem* hResource) = 0;

	/** Executed every tick. 
		@param[in]	dt	Delta time since last tick (seconds). */
	virtual void OnTick(float dt) = 0;

	/** Executed before frame is drawn. Used to submit draw instructions to renderer. */
	virtual void OnDraw(RenderSystem* hRender) = 0;

	/**	Returns initialization properties available in the map file or editor. */
	virtual void GetPropertyInfo(SPropertyInfo*& propertyInfo, uint32& propertyCount) const = 0;

	/** Returns pointers to member variables described by GetProperties(). */
	virtual void GetPropertyHandles(void**& handles) = 0;

	uint32 GetEnabledEventFlags() const;

	bool IsPendingKill() const;

protected:
	uint32 enabledEventFlags;
	bool bPendingKill;
};
