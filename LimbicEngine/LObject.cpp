#include "LObject.h"

LObject::LObject()
{
	bIsTickEnabled = false;
}

void LObject::SetTickEnabled(bool bIsTickEnabled)
{
	this->bIsTickEnabled = bIsTickEnabled;
}

void LObject::SetGameManagers(SGameManagers* managers)
{
	this->managers = managers;
}

bool LObject::IsTickEnabled()
{
	return bIsTickEnabled;
}

World* LObject::GetWorld()
{
	return managers->world;
}

ResourceManager* LObject::GetResourceManager()
{
	return managers->resources;
}

Game* LObject::GetGame()
{
	return managers->game;
}

SGameManagers* LObject::GetGameManagers()
{
	return managers;
}