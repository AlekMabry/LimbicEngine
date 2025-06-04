#pragma once

#include <Entity/EEntity.h>

class EPlayer : public EEntity
{
public:
	EPlayer(Game& game);

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	std::map<std::string, SPropertyInfo> GetPropertyInfo() override;

	vec3 position;

protected:
	mat4 transform;
};
