#include "LEntityComponent.h"

LEntityComponent::LEntityComponent()
{
	this->parent = nullptr;
	SetTickEnabled(false);
}

LEntityComponent::~LEntityComponent()
{
	for (LEntityComponent* child : children)
	{
		delete child;
	}
}

template <class TComponentClass>
TComponentClass* LEntityComponent::CreateChild(TComponentClass componentClass)
{
	static_assert(
		std::is_base_of<LEntityComponent, componentClass>::value, "Child object should be derived from LEntityComponent!");
	LEntityComponent* child = new TComponentClass();
	child->SetGameManagers(GetGameManagers());
	child->parent = this;
	children.push_back(child);
	return child;
}

std::vector<LEntityComponent*>* LEntityComponent::GetChildren()
{
	return &children;
}