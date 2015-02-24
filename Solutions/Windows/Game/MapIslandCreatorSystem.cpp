#include "MapIslandCreatorSystem.h"


MapIslandCreatorSystem::MapIslandCreatorSystem()
{
}


void MapIslandCreatorSystem::Initialize()
{
	SetSystemName("Map Island Creator System");

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("TileComp", ECSL::FilterType::Mandatory);
}

void MapIslandCreatorSystem::Update(float dt)
{
	auto entities = *GetEntities();

	//kod här

	/*Tar bort alla entiteter*/
	for (auto entity : entities)
	{
		RemoveComponentFrom("TileComp", entity);
	}
}

void MapIslandCreatorSystem::OnEntityAdded(unsigned int _entityId)
{

}

void MapIslandCreatorSystem::OnEntityRemoved(unsigned int _entityId)
{

}

MapIslandCreatorSystem::~MapIslandCreatorSystem()
{
}
