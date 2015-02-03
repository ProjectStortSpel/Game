--	This file will contain
--	components that are needed
--	for map components

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
RiverComponent:AddVariable("DirX", ByteSize.Int)
RiverComponent:AddVariable("DirZ", ByteSize.Int)
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
local MapSizeComponent = ComponentType()
MapSizeComponent.Name = "MapSize"
MapSizeComponent.TableType = TableType.Map
MapSizeComponent:AddVariable("X", ByteSize.Int)
MapSizeComponent:AddVariable("Y", ByteSize.Int)
worldCreator:AddComponentType(MapSizeComponent)