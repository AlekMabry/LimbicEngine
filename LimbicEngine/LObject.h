#pragma once

#include "Game.h"

class LObject
{
public:
	LObject();

	virtual void OnTick(float dt);

	void SetTickEnabled(bool bIsTickEnabled);

	void SetGameManagers(SGameManagers* managers);

	bool IsTickEnabled();

	World* GetWorld();

	ResourceManager* GetResourceManager();

	Game* GetGame();

	SGameManagers* GetGameManagers();

private:
	SGameManagers* managers;
	bool bIsTickEnabled;
};
