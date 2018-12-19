#pragma once

#include "player.hpp"

player::player(GLFWwindow* window) {
	windowPointer = window;
	mouseSpeed = 0.005f;
	pos.x = 256;
	pos.y = 256;
	pos.z = 256;
}

void player::update() {
	double xpos, ypos;
	glfwGetCursorPos(windowPointer, &xpos, &ypos);
	glfwSetCursorPos(windowPointer, 1024 / 2, 768 / 2);

	dir += mouseSpeed * float(1024 / 2 - xpos);
	pitch += mouseSpeed * float(768 / 2 - ypos);

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
	if (glfwGetKey(windowPointer, GLFW_KEY_UP) == GLFW_PRESS) {
		pos += dirVector * (float) 2; // deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(windowPointer, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pos -= dirVector * (float) 2; // deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(windowPointer, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		pos += rightVector * (float) 2; // deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(windowPointer, GLFW_KEY_LEFT) == GLFW_PRESS) {
		pos -= rightVector * (float) 2; // deltaTime * speed;
	}
	
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
