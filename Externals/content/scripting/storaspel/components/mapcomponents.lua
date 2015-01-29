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
RiverComponent:AddVariable("DirX", ByteSize.Int)
RiverComponent:AddVariable("DirZ", ByteSize.Int)
worldCreator:AddComponentType(RiverComponent)

-- Checkpoint Component
local CheckpointComponent = ComponentType()
CheckpointComponent.Name = "Checkpoint"
CheckpointComponent.TableType = TableType.Map
CheckpointComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(CheckpointComponent)

-- Finishpoint Component
local FinishpointComponent = ComponentType()
FinishpointComponent.Name = "Finishpoint"
FinishpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(FinishpointComponent)

-- Hole and Out Component
local VoidComponent = ComponentType()
VoidComponent.Name = "Void"
VoidComponent.TableType = TableType.Map
worldCreator:AddComponentType(VoidComponent)

-- Not Walkable Component, no unit can end up here, for example a stone or tree occupying a tile
local NotWalkableComponent = ComponentType()
NotWalkableComponent.Name = "NotWalkable"
NotWalkableComponent.TableType = TableType.Map
worldCreator:AddComponentType(NotWalkableComponent)

-- Map Size Component, added to exactly entity to signal how big the map is.
local MapSizeComponent = ComponentType()
MapSizeComponent.Name = "MapSize"
MapSizeComponent.TableType = TableType.Map
MapSizeComponent:AddVariable("X", ByteSize.Int)
MapSizeComponent:AddVariable("Y", ByteSize.Int)
worldCreator:AddComponentType(MapSizeComponent)
