#pragma once

#include "IRenderer.h"


class RendererOpenGL : IRenderer
{
public:
	RendererOpenGL();
	uint32 LoadShader();

protected:
};
