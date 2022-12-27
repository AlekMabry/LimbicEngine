#pragma once

#include "LEntityComponent.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class LWorldComponent : public LEntityComponent
{
public:
	LWorldComponent();

	void SetTransform(vec3 scale, vec4 rotation, vec3 position);

	void SetScale(vec3 scale);

	void SetPosition(vec3 position);

	void SetRotation(vec4 rotation);

	mat4 GetLocalTransform() const;

	bool IsRenderEnabled();

	bool IsEditorRenderEnabled();

	bool IsCollisionEnabled();

protected:
	void RecalculateTransform();

	mat4 transform;
	vec3 scale;
	vec3 position;
	vec4 rotation;

	bool bRenderEnabled;
	bool bEditorRenderEnabled;
	bool bCollisionEnabled;
};
