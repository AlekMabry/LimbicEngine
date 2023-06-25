#pragma once

#include <Entity/EEntity.h>

class ETestCamera : public EEntity
{
public:
	ETestCamera(Game& game);

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	void GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount) override;

protected:
	float angle;
};
