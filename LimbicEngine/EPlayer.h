#pragma once

#include "EEntity.h"

class EPlayer : public EEntity
{
public:
	EPlayer();

	void OnInit(ResourceSystem* hResource) override;

	void OnTick(float dt) override;

	void OnDraw(RenderSystem* hRender) override;

	void GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount) override;

protected:
	float angle;
};
