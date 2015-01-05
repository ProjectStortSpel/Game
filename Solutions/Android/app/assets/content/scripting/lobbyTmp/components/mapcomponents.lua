
-- Map Position Component
local MapPositionComponent = ComponentType()
MapPositionComponent.Name = "MapPosition"
MapPositionComponent.TableType = TableType.Array
MapPositionComponent:AddVariable("X", 4)
MapPositionComponent:AddVariable("Y", 4)
worldCreator:AddComponentType(MapPositionComponent)

-- Water Component
local WaterComponent = ComponentType()
WaterComponent.Name = "Water"
WaterComponent.TableType = TableType.Array
WaterComponent:AddVariable("DirX", 4)
WaterComponent:AddVariable("DirY", 4)
worldCreator:AddComponentType(WaterComponent)

-- Checkpoint Component
local CheckpointComponent = ComponentType()
CheckpointComponent.Name = "Checkpoint"
CheckpointComponent.TableType = TableType.Array
CheckpointComponent:AddVariable("Number", 4)
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