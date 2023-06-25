#pragma once

#include <LimbicTypes.h>

#include <string>

#define LPROPERTY(propertyInfo, variable, editorLabel, typeEnum) \
	propertyInfo.name = std::string(LSTRINGIFY(variable));       \
	propertyInfo.label = std::string(editorLabel);               \
	propertyInfo.type = typeEnum;                                \
	propertyInfo.handle = static_cast<void*>(&variable);

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