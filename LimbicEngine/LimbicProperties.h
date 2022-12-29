#pragma once

#include "LimbicTypes.h"
#include <string>

enum EPropertyType
{
	ePropertyTypeFilename,
	ePropertyTypeVec4,
	ePropertyTypeVec3,
	ePropertyTypeVec2
};

struct SPropertyInfo
{
	std::string name;			/** Identifier in map files and code. */
	std::string label;			/** Fancy identifier for editor. */
	EPropertyType type;			/** Type of property. */
};