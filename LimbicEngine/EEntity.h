#pragma once

#include "LObject.h"
#include "LEntityComponent.h"

class EEntity : public LObject
{
public:
	EEntity();

	~EEntity();

	LEntityComponent* GetRoot() const;

	bool IsPendingKill() const;

protected:
	bool bPendingKill;
	LEntityComponent* root;
};
