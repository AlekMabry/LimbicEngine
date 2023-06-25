#include <Entity/EEntity.h>
#include <Game.h>

EEntity::EEntity(Game& game)
	: enabledEventFlags(0), bPendingKill(false), pGame(&game)
{
	pIO = pGame->GetIOSystem();
	pResource = pGame->GetResourceSystem();
	pRender = pGame->GetRenderSystem();
	pWorld = pGame->GetWorldSystem();
}

uint32 EEntity::GetEnabledEventFlags() const
{
	return enabledEventFlags;
}

bool EEntity::IsPendingKill() const
{
	return bPendingKill;
}