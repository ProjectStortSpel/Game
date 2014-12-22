
-- Map Position Component
local MapPositionComponent = ComponentType()
MapPositionComponent.Name = "MapPosition"
MapPositionComponent.TableType = TableType.Array
MapPositionComponent:AddVariable("X", ByteSize.Int)
MapPositionComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(MapPositionComponent)

-- Water Component
local RiverComponent = ComponentType()
RiverComponent.Name = "River"
RiverComponent.TableType = TableType.Array
RiverComponent:AddVariable("DirX", ByteSize.Int)
RiverComponent:AddVariable("DirZ", ByteSize.Int)
worldCreator:AddComponentType(RiverComponent)

-- Checkpoint Component
local CheckpointComponent = ComponentType()
CheckpointComponent.Name = "Checkpoint"
CheckpointComponent.TableType = TableType.Array
CheckpointComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(CheckpointComponent)

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