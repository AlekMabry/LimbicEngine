#pragma once

#include "LObject.h"
#include "LEntityComponent.h"

class EEntity : public LObject
{
public:
	EEntity();

	/** Recommended way to spawn a root component. */
	template <class TComponentClass>
	TComponentClass* CreateRoot(TComponentClass componentClass);

	LEntityComponent* GetRoot() const;

	bool IsPendingKill() const;

protected:
	bool bTickEnabled;
	bool bPendingKill;
	LEntityComponent* root;
	SGameManagers* managers;
};
