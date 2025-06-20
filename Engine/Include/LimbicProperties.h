#pragma once

#include <LimbicTypes.h>

#include <string>

// Define std::map<std::string, SPropertyInfo> properties before using
#define LPROPERTY(variable, editorLabel, typeEnum)			\
	properties[LSTRINGIFY(variable)] =						\
		SPropertyInfo(LSTRINGIFY(variable), editorLabel,	\
		typeEnum, static_cast<void*>(&variable));

enum EPropertyType
{
	ePropertyTypeFilename,
	ePropertyTypeVec4,
	ePropertyTypeVec3,
	ePropertyTypeVec2,
	ePropertyTypeBool
};

struct SPropertyInfo
{
	std::string name;	/** Identifier in map files and code. */
	std::string label;	/** Fancy identifier for editor. */
	EPropertyType type; /** Type of property. */
	void* handle;		/** Handle of variable this property describes. */
};