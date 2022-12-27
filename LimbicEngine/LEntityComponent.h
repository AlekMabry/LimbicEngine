#pragma once

#include "LObject.h"
#include <vector>

class LEntityComponent : public LObject
{
public:
	LEntityComponent();

	~LEntityComponent();

	/** Create and attach child component to this component. */
	template <typename TComponentClass>
	TComponentClass* CreateChild();

	std::vector<LEntityComponent*>* GetChildren();

protected:
	LEntityComponent* parent;
	std::vector<LEntityComponent*> children;
};