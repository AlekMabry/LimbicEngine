#pragma once

#include "LimbicTypes.h"
#include <memory>
#include <vector>

enum EComponentType {
	ComponentType_EntityComponent,
	ComponentType_SceneComponent
};

class EntityComponent
{
	virtual EComponentType GetType() const;

	void SetTickEnabled(bool bTickEnabled);

	bool IsTickEnabled() const;

	/** Called by game if tick is enabled.
		@param[in] dt Delta time since last tick (in ms). */
	virtual void OnTick(uint32 dt);

	/** Attach to a parent actor. */
	void AttachChild(std::shared_ptr<EntityComponent> child);

protected:
	bool bTickEnabled;
	EntityComponent* parent;
	std::vector<std::shared_ptr<EntityComponent>> children;
};

