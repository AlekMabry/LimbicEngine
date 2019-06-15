#ifndef PLAYER_H
#define PLAYER_H

#include "common.hpp"
#include "render.hpp"

class player {
private:

public:
	vec3 pos;
	float dir;
	float pitch;
	float mouseSpeed;

	GLFWwindow* window;
	render* renderHandle;

	vec3 dirVector;
	vec3 rightVector;
	vec3 upVector;

	player(GLFWwindow* wHandle, render* rHandle);
	void update();
	void pushPerspective();
	vec3 getDirection();
	vec3 getPosition();
	vec3 getRight();
	vec3 getUp();
};

#endif