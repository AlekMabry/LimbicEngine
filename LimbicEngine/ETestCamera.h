#pragma once

#include "EEntity.h"

class ETestCamera : public EEntity
{
public:
	ETestCamera();

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	void GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount) override;

protected:
	float angle;
};
