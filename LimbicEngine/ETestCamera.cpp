#include "ETestCamera.h"
#define GLM_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

ETestCamera::ETestCamera()
{
	enabledEventFlags = ENTITY_EVENT_ENABLED_TICK | ENTITY_EVENT_ENABLED_DRAW;
}

void ETestCamera::OnInit()
{
	angle = 0.0f;
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
	mat4 proj = glm::perspective(glm::radians(120.0f), 16.0f / 9.0f, 0.1f, 128.0f);
	proj[1][1] *= -1;

	hRender->DrawSetCamera(proj * view);
}

void ETestCamera::GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount)
{
	propertyCount = 0;
}