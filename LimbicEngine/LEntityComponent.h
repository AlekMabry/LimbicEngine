#pragma once

#include "LObject.h"
#include <vector>

class LEntityComponent : LObject
{
public:
	LEntityComponent();

	~LEntityComponent();

	/** Create and attach child component to this component. */
	template <class TComponentClass>
	TComponentClass* CreateChild(TComponentClass componentClass);

	std::vector<LEntityComponent*>* GetChildren();

protected:
	LEntityComponent* parent;
	std::vector<LEntityComponent*> children;
};