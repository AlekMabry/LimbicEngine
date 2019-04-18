#ifndef RENDER_HPP
#define RENDER_HPP

#include "io.hpp"
#include "common.hpp"
#include <vector>

class render
{
private:
public:
	io* statusStream;
	
	// Settings
	int screenWidth;
	int screenHeight;
	float FOV;

	virtual int openWAD(const char* fname);
	virtual int addMatWAD(const char* texname, vec2 dimensions, int &id, int type);
	virtual int closeWAD();
	virtual int pushVertices(int iMat, std::vector<float> &vertData, std::vector<float> &uvData, std::vector<float> &normData);
	virtual int setWorldView(vec3 pos, vec3 dir, vec3 up, float fov);
	virtual void render3D();
	virtual int uploadFramebuffer();
};

#endif