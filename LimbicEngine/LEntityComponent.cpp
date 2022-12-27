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

template <typename TComponentClass>
TComponentClass* LEntityComponent::CreateChild()
{
	static_assert(
		std::is_base_of<LEntityComponent, TComponentClass>::value, "Child object should be derived from LEntityComponent!");
	LEntityComponent* child = new TComponentClass();
	child->parent = this;
	children.push_back(child);
	return child;
}

std::vector<LEntityComponent*>* LEntityComponent::GetChildren()
{
	return &children;
}