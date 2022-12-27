#include "EEntity.h"

EEntity::EEntity()
{
	bPendingKill = false;
	SetTickEnabled(true);
}

EEntity::EEntity()
{
	delete root;
}

template <class TComponentClass>
TComponentClass* EEntity::CreateRoot(TComponentClass componentClass)
{
	if (root == nullptr)
	{
		root = new TComponentClass();
	}
	root->SetGameManagers(GetGameManagers());
	return root;
}

LEntityComponent* EEntity::GetRoot() const
{
	return root;
}

bool EEntity::IsPendingKill() const
{
	return bPendingKill;
}