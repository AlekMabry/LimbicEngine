#include "render.hpp"

int render::openWAD(const char* fname)
{
	return -1;
}

int render::addMatWAD(const char* texname, vec2 dimensions, int &id, int type)
{
	return -1;
}

int render::closeWAD()
{
	return -1;
}

int render::pushVertices(int iMat, std::vector<float> &vertData, std::vector<float> &uvData)
{
	return -1;
}

int render::setWorldView(vec3 pos, vec3 dir, vec3 up, float fov)
{
	return -1;
}

void render::render3D()
{
}

int render::uploadFramebuffer()
{
	return -1;
}