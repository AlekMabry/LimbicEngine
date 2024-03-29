#include <Game.h>
#include <System/WorldSystem.h>

#include <Entity/EEntity.h>
#include <Entity/ETestCamera.h>
#include <Entity/EStaticWorldGeometry.h>
#include <Entity/EPlayer.h>

#include "rapidjson/document.h"

#include <stdlib.h>
#include <array>
#include <iostream>

WorldSystem::WorldSystem(Game& game)
	: pGame(&game)
{
	LREGISTER(EStaticWorldGeometry)
	LREGISTER(ETestCamera)
	LREGISTER(EPlayer)
}

WorldSystem::~WorldSystem()
{
}

void WorldSystem::OnTick(float dt)
{
	for (const auto& entity : entities)
	{
		entity->OnTick(dt);
	}
}

void WorldSystem::OnDraw()
{
	for (const auto& entity : entities)
	{
		entity->OnDraw();
	}
}

void WorldSystem::LoadFromJSON(const char* filename)
{
	FILE* file;
	file = fopen(filename, "rb");
	if (file == nullptr)
	{
		std::cout << "[ERROR] Unable to open map file!\n";
		return;
	}

	fseek(file, 0, SEEK_END);
	uint64 size = ftell(file);
	std::vector<char> buffer(size + 1);

	fseek(file, 0, SEEK_SET);
	fread(buffer.data(), sizeof(char), size, file);
	buffer[buffer.size() - 1] = NULL;

	fclose(file);

	rapidjson::Document document;
	document.Parse(buffer.data());

	// Create data structures to store entity property info (json property identifiers,
	// property types, and property pointers in spawned entity)
	std::array<SPropertyInfo, 16> entityPropertyInfo;
	uint32 entityPropertyCount;

	rapidjson::Value& entityJsonArray = document["Entities"];
	for (size_t i = 0; i < entityJsonArray.Size(); i++)
	{
		// Get entity Type string and Properties JSON object
		rapidjson::Value& entityJsonObject = entityJsonArray[static_cast<rapidjson::SizeType>(i)];
		std::string entityTypeString = entityJsonObject["Type"].GetString();
		rapidjson::Value& entityPropertiesJsonArray = entityJsonObject["Properties"];

		// Spawn entity using spawn function in the lookup table
		EEntity* spawned = NewEntity[entityTypeString]();

		// Ask entity for properties information
		spawned->GetPropertyInfo(entityPropertyInfo.data(), entityPropertyCount);

		// For each property the entity requests, find it in the JSON file and write to entity with property handles.
		for (uint32 p = 0; p < entityPropertyCount; p++)
		{
			rapidjson::Value& propertyJsonObject = entityPropertiesJsonArray[entityPropertyInfo[p].name.c_str()];
			switch (entityPropertyInfo[p].type)
			{
				case ePropertyTypeFilename:
					*static_cast<std::string*>(entityPropertyInfo[p].handle) = propertyJsonObject.GetString();
					break;
				case ePropertyTypeVec4:
					static_cast<vec4*>(entityPropertyInfo[p].handle)->x = propertyJsonObject["x"].GetFloat();
					static_cast<vec4*>(entityPropertyInfo[p].handle)->y = propertyJsonObject["y"].GetFloat();
					static_cast<vec4*>(entityPropertyInfo[p].handle)->z = propertyJsonObject["z"].GetFloat();
					static_cast<vec4*>(entityPropertyInfo[p].handle)->w = propertyJsonObject["w"].GetFloat();
					break;
				case ePropertyTypeVec3:
					static_cast<vec3*>(entityPropertyInfo[p].handle)->x = propertyJsonObject["x"].GetFloat();
					static_cast<vec3*>(entityPropertyInfo[p].handle)->y = propertyJsonObject["y"].GetFloat();
					static_cast<vec3*>(entityPropertyInfo[p].handle)->z = propertyJsonObject["z"].GetFloat();
					break;
				case ePropertyTypeVec2:
					static_cast<vec2*>(entityPropertyInfo[p].handle)->x = propertyJsonObject["x"].GetFloat();
					static_cast<vec2*>(entityPropertyInfo[p].handle)->y = propertyJsonObject["y"].GetFloat();
					break;
			}
		}
	}
}

void WorldSystem::GetEntities(EEntity**& entities, uint32& entityCount)
{
	entities = this->entities.data();
	entityCount = static_cast<uint32>(this->entities.size());
}