#pragma once

#include "EntityComponent.h"
#include "glm/mat4x4.hpp"

using namespace glm;

class WorldComponent :
	public EntityComponent
{
	/** Get transform matrix of this component.
		@param[in,out] transform Output transform matrix. */
	void GetComponentTransform(mat4x4 &transform) const;

	/** Get's transform in worldspace. */
	void GetWorldTransform(mat4x4 &transform) const;
	
	void SetPosition(vec3 position);

	void SetRotation(vec3 rotation);

	void SetScale(vec3 scale);

protected:
	mat4x4 transform;
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

