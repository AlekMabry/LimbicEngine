#ifndef PLAYER_H
#define PLAYER_H

#include "common.hpp"

class player {
private:

public:
	vec3 pos;
	float dir;
	float pitch;
	float mouseSpeed;
	GLFWwindow* windowPointer;

	vec3 dirVector;
	vec3 rightVector;
	vec3 upVector;

	player(GLFWwindow * window);
	void update();
	vec3 getDirection();
	vec3 getPosition();
	vec3 getRight();
	vec3 getUp();
};

#endif