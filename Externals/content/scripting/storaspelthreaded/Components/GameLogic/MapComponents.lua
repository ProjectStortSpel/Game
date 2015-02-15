--	This file will contain
--	components that are needed
--	for map components

-- Create Map Component
local CreateMapComponent = ComponentType()
CreateMapComponent.Name = "CreateMap"
CreateMapComponent.TableType = TableType.Map
CreateMapComponent:AddVariable("MapName", ByteSize.Text)
worldCreator:AddComponentType(CreateMapComponent)

-- Map Position Component
local MapPositionComponent = ComponentType()
MapPositionComponent.Name = "MapPosition"
MapPositionComponent.TableType = TableType.Map
MapPositionComponent.SyncNetwork = true
MapPositionComponent:AddVariable("X", ByteSize.Int)
MapPositionComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(MapPositionComponent)

-- Water Component
local RiverComponent = ComponentType()
RiverComponent.Name = "River"
RiverComponent.TableType = TableType.Map
RiverComponent.SyncNetwork = true
RiverComponent:AddVariable("DirX", ByteSize.Int)
RiverComponent:AddVariable("DirZ", ByteSize.Int)
RiverComponent:AddVariable("Speed", ByteSize.Int)
worldCreator:AddComponentType(RiverComponent)

-- Checkpoint Component
local CheckpointComponent = ComponentType()
CheckpointComponent.Name = "Checkpoint"
CheckpointComponent.TableType = TableType.Map
CheckpointComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(CheckpointComponent)

-- CheckpointId Component
local CheckpointIdComponent = ComponentType()
CheckpointIdComponent.Name = "CheckpointId"
CheckpointIdComponent.TableType = TableType.Map
CheckpointIdComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(CheckpointIdComponent)

-- Tile Component
local TileComponent = ComponentType()
TileComponent.Name = "TileComp"
TileComponent.TableType = TableType.Map
worldCreator:AddComponentType(TileComponent)

-- Finishpoint Component
local FinishpointComponent = ComponentType()
FinishpointComponent.Name = "Finishpoint"
FinishpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(FinishpointComponent)

-- Edge?
-- Hole and Out Component
local VoidComponent = ComponentType()
VoidComponent.Name = "Void"
VoidComponent.TableType = TableType.Map
worldCreator:AddComponentType(VoidComponent)

-- Not Walkable Component
local NotWalkableComponent = ComponentType()
NotWalkableComponent.Name = "NotWalkable"
NotWalkableComponent.TableType = TableType.Map
worldCreator:AddComponentType(NotWalkableComponent)

-- Available Spawnpoint
local AvailableSpawnpoint = ComponentType()
AvailableSpawnpoint.Name = "AvailableSpawnpoint"
AvailableSpawnpoint.TableType = TableType.Map
AvailableSpawnpoint:AddVariable("X", ByteSize.Int)
AvailableSpawnpoint:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(AvailableSpawnpoint)

-- Map Size Component, added to exactly entity to signal how big the map is.
local MapSpecsComponent = ComponentType()
MapSpecsComponent.Name = "MapSpecs"
MapSpecsComponent.SyncNetwork = true
MapSpecsComponent.TableType = TableType.Map
MapSpecsComponent:AddVariable("NoOfSpawnpoints", ByteSize.Int)
MapSpecsComponent:AddVariable("NoOfCheckpoints", ByteSize.Int)
MapSpecsComponent:AddVariable("SizeX", ByteSize.Int)
MapSpecsComponent:AddVariable("SizeY", ByteSize.Int)
worldCreator:AddComponentType(MapSpecsComponent)

--	GenerateMap
local GenerateMap = ComponentType()
GenerateMap.Name = "GenerateMap"
GenerateMap.TableType = TableType.Map
worldCreator:AddComponentType(GenerateMap)