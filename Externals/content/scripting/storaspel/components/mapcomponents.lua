
-- Map Position Component
local MapPositionComponent = ComponentType()
MapPositionComponent.Name = "MapPosition"
MapPositionComponent.TableType = TableType.Map
MapPositionComponent:AddVariable("X", ByteSize.Int)
MapPositionComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(MapPositionComponent)

-- Water Component
local RiverComponent = ComponentType()
RiverComponent.Name = "River"
MapPositionComponent.SyncNetwork = true
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