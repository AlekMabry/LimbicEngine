#pragma once

#include <Entity/EEntity.h>

class ETestCamera : public EEntity
{
public:
	ETestCamera(Game& game);

	void OnInit() override;

	void OnTick(float dt) override;

	void OnDraw() override;

	std::map<std::string, SPropertyInfo> GetPropertyInfo() override;

protected:
	float angle;
};
