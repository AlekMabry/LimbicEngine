#pragma once

#include "LimbicTypes.h"

#include <string>

#define LPROPERTY(propertyInfo, nameLiteral, labelLiteral, typeEnum, propertyVariable) \
	propertyInfo.name = std::string(nameLiteral);                                      \
	propertyInfo.label = std::string(labelLiteral);                                    \
	propertyInfo.type = typeEnum;                                                      \
	propertyInfo.handle = static_cast<void*>(&propertyVariable);

enum EPropertyType
{
	ePropertyTypeFilename,
	ePropertyTypeVec4,
	ePropertyTypeVec3,
	ePropertyTypeVec2
};

struct SPropertyInfo
{
	std::string name;	/** Identifier in map files and code. */
	std::string label;	/** Fancy identifier for editor. */
	EPropertyType type; /** Type of property. */
	void* handle;		/** Handle of variable this property describes. */
};