#include "LObject.h"

LObject::LObject()
{
	bIsTickEnabled = false;
}

void LObject::OnTick(float dt)
{
}

void LObject::SetTickEnabled(bool bIsTickEnabled)
{
	this->bIsTickEnabled = bIsTickEnabled;
}

bool LObject::IsTickEnabled()
{
	return bIsTickEnabled;
}