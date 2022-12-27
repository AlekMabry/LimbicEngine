#include "LWorldComponent.h"

LWorldComponent::LWorldComponent() : LEntityComponent()
{
	scale = vec3(1.0f, 1.0f, 1.0f);
	position = vec3(0.0f, 0.0f, 0.0f);
	rotation = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	RecalculateTransform();
	bRenderEnabled = false;
	bEditorRenderEnabled = false;
	bCollisionEnabled = true;
}

void LWorldComponent::SetTransform(vec3 scale, vec4 rotation, vec3 position)
{
	this->scale = scale;
	this->position = position;
	this->rotation = rotation;
}

void LWorldComponent::SetScale(vec3 scale)
{
	this->scale = scale;
	RecalculateTransform();
}

void LWorldComponent::SetPosition(vec3 position)
{
	this->position = position;
	RecalculateTransform();
}

void LWorldComponent::SetRotation(vec4 rotation)
{
	this->rotation = rotation;
	RecalculateTransform();
}

void LWorldComponent::RecalculateTransform()
{
	transform = glm::scale(mat4(1.0f), scale);
	transform = glm::rotate(transform, rotation.w, vec3(rotation));
	transform = glm::translate(transform, position);
}

mat4 LWorldComponent::GetLocalTransform() const
{
	return transform;
}

bool LWorldComponent::IsRenderEnabled()
{
	return bRenderEnabled;
}

bool LWorldComponent::IsEditorRenderEnabled()
{
	return bEditorRenderEnabled;
}

bool LWorldComponent::IsCollisionEnabled()
{
	return bCollisionEnabled;
}