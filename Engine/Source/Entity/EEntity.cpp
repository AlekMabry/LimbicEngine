#include <Entity/EEntity.h>
#include <Game.h>

EEntity::EEntity(Game& game)
	: enabledEventFlags(0), bPendingKill(false), pGame(&game)
{
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

uint32 EEntity::GetID() const
{
	return id;
}

std::string& EEntity::GetName()
{
	return name;
}