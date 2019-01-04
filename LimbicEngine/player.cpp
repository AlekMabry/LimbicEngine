#pragma once

#include "player.hpp"

player::player(GLFWwindow* wHandle, render* rHandle) {
	window = wHandle;
	renderHandle = rHandle;
	mouseSpeed = 0.005f;
	pos.x = 256;
	pos.y = 256;
	pos.z = 128;
}

void player::update() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, 1280 / 2, 720 / 2);

	dir += mouseSpeed * float(1280 / 2 - xpos);
	pitch += mouseSpeed * float(720 / 2 - ypos);

	// Unit vector in the direction the player is facing
	dirVector.x = cos(dir) * cos(pitch);
	dirVector.y = sin(dir) * cos(pitch);
	dirVector.z = sin(pitch);

	// Right vector
	rightVector.x = cos(dir - 3.14f / 2.0f);
	rightVector.y = sin(dir - 3.14f / 2.0f);
	rightVector.z = 0;

	// Up vector
	upVector = cross(rightVector, dirVector);
	
	
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pos += dirVector * (float) 2; // deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pos -= dirVector * (float) 2; // deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		pos += rightVector * (float) 2; // deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		pos -= rightVector * (float) 2; // deltaTime * speed;
	}
	
}

void player::pushPerspective()
{
	renderHandle->setWorldView(getPosition(), getDirection(), getUp() ,90);
}

vec3 player::getDirection()
{
	return dirVector;
}

vec3 player::getPosition()
{
	return pos;
}

vec3 player::getRight()
{
	return rightVector;
}

vec3 player::getUp()
{
	return upVector;
}
