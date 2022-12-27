#include "EEntity.h"

EEntity::EEntity()
{
	bPendingKill = false;
	SetTickEnabled(false);
}

EEntity::~EEntity()
{
	delete root;
}

LEntityComponent* EEntity::GetRoot() const
{
	return root;
}

bool EEntity::IsPendingKill() const
{
	return bPendingKill;
}