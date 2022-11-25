#pragma once
#include <memory>
#include <vector>

// Types of resources
// Folder
// Texture
// Mesh

class RResource
{
public:
	RResource(RResource* parent);
	void addChild();

protected:
	std::vector<std::unique_ptr<RResource>> children;
};
