#include "Entity/ETestCamera.h"
#include "System/RenderSystem.h"
#include "Renderer/RView.h"

#define GLM_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

ETestCamera::ETestCamera(Game& game)
	: EEntity(game), angle(0.0f)
{
	enabledEventFlags = ENTITY_EVENT_ENABLED_TICK | ENTITY_EVENT_ENABLED_DRAW;
}

void ETestCamera::OnInit()
{
}

void ETestCamera::OnTick(float dt)
{
	angle += dt * glm::radians(15.0f);
	if (angle > glm::radians(360.0f))
	{
		angle -= glm::radians(360.0f);
	}
}

void ETestCamera::OnDraw()
{
	vec3 position = vec3(3.0f * cos(angle), 3.0f * sin(angle), 1.8f);
	mat4 view = glm::lookAt(position, vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f));
	auto windowView = pRender->GetWindowView("main");
	windowView.second->SetViewTransform(view);
	windowView.second->SetProjection(0.1f, 128.0f, 120.0f);
}

std::map<std::string, SPropertyInfo> ETestCamera::GetPropertyInfo()
{
	return {};
}