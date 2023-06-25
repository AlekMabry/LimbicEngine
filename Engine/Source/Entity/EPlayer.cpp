#include <Entity/EPlayer.h>

#include <System/IOSystem_InputBinds.h>
#include <System/RenderSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE

#include <System/IOSystem.h>
#include <glm/gtc/matrix_transform.hpp>

enum EPlayerActions
{
	ePlayerActionForward,
	ePlayerActionBackward,
	ePlayerActionLeft,
	ePlayerActionRight,
};

EPlayer::EPlayer(Game& game)
	: EEntity(game), position(0), transform(glm::mat4())
{
	enabledEventFlags = ENTITY_EVENT_ENABLED_TICK | ENTITY_EVENT_ENABLED_DRAW;
}

void EPlayer::OnInit()
{
	std::array<EKey,4> keys;
	keys[ePlayerActionForward] = eKey_W;
	keys[ePlayerActionBackward] = eKey_S;
	keys[ePlayerActionLeft] = eKey_A;
	keys[ePlayerActionRight] = eKey_D;

	pIO->BindActions(keys.data(), static_cast<uint32>(keys.size()));
}

void EPlayer::OnTick(float dt)
{
	vec3 moveVec = vec3(0.0f);
	
	if (pIO->IsActionHeld(ePlayerActionForward))
		moveVec += vec3(1.0f, 0.0f, 0.0f);

	if (pIO->IsActionHeld(ePlayerActionBackward))
		moveVec += vec3(-1.0f, 0.0f, 0.0f);

	if (pIO->IsActionHeld(ePlayerActionLeft))
		moveVec += vec3(0.0f, 1.0f, 0.0f);

	if (pIO->IsActionHeld(ePlayerActionRight))
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

	pRender->DrawSetCamera(proj * view);
}

void EPlayer::GetPropertyInfo(SPropertyInfo* propertyInfo, uint32& propertyCount)
{
	LPROPERTY(propertyInfo[0], position, "Position", ePropertyTypeVec3);

	propertyCount = 1;
}