#include <Entity/EEntity.h>

EEntity::EEntity()
{
	bPendingKill = false;
	enabledEventFlags = 0;
}

uint32 EEntity::GetEnabledEventFlags() const
{
	return enabledEventFlags;
}

bool EEntity::IsPendingKill() const
{
	return bPendingKill;
}