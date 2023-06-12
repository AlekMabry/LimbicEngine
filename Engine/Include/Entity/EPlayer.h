#pragma once

#include <Entity/EEntity.h>

class EPlayer : public EEntity
{
public:
	EPlayer();

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	void GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount) override;

	vec3 position;

protected:
	mat4 transform;
};
