#pragma once
#ifndef PC_GL_RENDER_H
#define PC_GL_RENDER_H

#ifdef WINDOWS
#include <windows.h>
#endif

#include "render.hpp"
#include "error.hpp"
#include "common.hpp"
#include <vector>

#pragma pack(push)
#pragma pack(1)

struct dwadheader_t
{
	char szMagic[4];	// Should be WAD2/WAD3
	int nDir;			// Number of directory entries
	int nDirOffset;		// Offset into directory
};

struct dentry_t
{
	int nFilePos;		// Offset in WAD
	int nDiskSize;		// Size in file
	int nSize;			// Uncompressed size
	char nType;			// Type of entry
	bool bCompression;	// 0 if none
	short nDummy;		// Not used
	char szName[16];	// Must be null terminated
};

#ifndef DTEXDATA_T
#define DTEXDATA_T
struct dtexdata_t
{
	char szName[16];  // Name of texture
	unsigned int nWidth, nHeight;     // Extends of the texture
	unsigned int nOffsets[4]; // Offsets to texture mipmaps BSPMIPTEX;
};
#endif

#pragma pack(pop)

struct d_material_opaque
{
	GLuint texture;		// Contains color and specular
	GLuint normal;		// Contains normals and roughness
	vec2 dimensions;	// Width and height of texture
	std::vector<int> size;		// Length of vertex data
	std::vector<GLuint> vertData;	// Contains array of vertex data
	std::vector<GLuint> uvData;	// Contains array of vertex data
	std::vector<GLuint> normData;	// Contains array of normal data
};

class pc_gl_render :public render
{
private:
	// Temporary/Ect
	GLuint tex_missing;

	// Shaders
	GLuint opaqueShader;
	GLuint deferred_firstPassShader;
	GLuint debugShader;

	// Four Quad VBO
	GLuint gl_fourQuad_vertData;
	GLuint gl_fourQuad_uvData;

	// One vao, multiple VAOs is a waste of state change
	GLuint gl_vao;

	GLFWwindow* window;
	mat4 world_gl_transform_projection;
	mat4 world_gl_transform_view;
	mat4 world_gl_transform_world_view;
	mat3 world_gl_normal_view;

	std::vector<d_material_opaque> matArray;

	// Stuff for load WADs
	FILE* wadFile;
	dwadheader_t wadHeader;
	dentry_t* wadEntryArray;
	int wadEntryArrayLen;

	// Framebuffers for Deferred Rendering
	GLuint gBuffer;
	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedoSpec;
	GLuint gAttachments[3];
	GLuint gDepthStencil;

public:
	pc_gl_render(io* inOutStream, int w, int h, float fov);
	int openWAD(const char* fname);
	int addMatWAD(const char* texname, vec2 dimensions, int &id, int type);
	int closeWAD();
	int pushVertices(int iMat, std::vector<float> &vertData, std::vector<float> &uvData, std::vector<float> &normData);
	int setWorldView(vec3 pos, vec3 dir, vec3 up, float fov);
	void render3D();
	int uploadFramebuffer();
	GLFWwindow* getWindow();
};

#endif