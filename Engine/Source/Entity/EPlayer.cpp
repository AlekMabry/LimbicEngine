#include <Entity/EPlayer.h>

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

enum EPlayerActions
{
	ePlayerActionForward,
	ePlayerActionBackward,
	ePlayerActionLeft,
	ePlayerActionRight,
};

EPlayer::EPlayer()
{
	enabledEventFlags = ENTITY_EVENT_ENABLED_TICK | ENTITY_EVENT_ENABLED_DRAW;
}

void EPlayer::OnInit()
{
	std::array<EInput,4> keys;
	keys[ePlayerActionForward] = eInput_W;
	keys[ePlayerActionBackward] = eInput_S;
	keys[ePlayerActionLeft] = eInput_A;
	keys[ePlayerActionRight] = eInput_D;

	hIO->BindActions(keys.data(), static_cast<uint32>(keys.size()));
}

void EPlayer::OnTick(float dt)
{
	vec3 moveVec = vec3(0.0f);
	
	if (hIO->IsActionHeld(ePlayerActionForward))
		moveVec += vec3(1.0f, 0.0f, 0.0f);

	if (hIO->IsActionHeld(ePlayerActionBackward))
		moveVec += vec3(-1.0f, 0.0f, 0.0f);

	if (hIO->IsActionHeld(ePlayerActionLeft))
		moveVec += vec3(0.0f, 1.0f, 0.0f);

	if (hIO->IsActionHeld(ePlayerActionRight))
		moveVec += vec3(0.0f, -1.0f, 0.0f);

	glm::normalize(moveVec);

	position += (moveVec * 2.0f * dt);
}

void EPlayer::OnDraw()
{
	vec3 eye = position + vec3(0.0, 0.0, 1.8f);
	mat4 view = glm::lookAt(eye, vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f));
	mat4 proj = glm::perspective(glm::radians(120.0f), 16.0f / 9.0f, 0.1f, 128.0f);
	proj[1][1] *= -1;

	//hRender->DrawSetCamera(proj * view);
}

void EPlayer::GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount)
{
	LPROPERTY(propertyInfo[0], position, "Position", ePropertyTypeVec3);

	propertyCount = 1;
}