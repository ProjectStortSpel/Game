-- Null Component
local NullComponent = ComponentType()
NullComponent.Name = "Null"
NullComponent.SyncNetwork = true
NullComponent.TableType = TableType.Map
worldCreator:AddComponentType(NullComponent)

-- Parent Component
local ParentComponent = ComponentType()
ParentComponent.Name = "Parent"
ParentComponent.TableType = TableType.Map
ParentComponent.SyncNetwork = true
ParentComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(ParentComponent)
-- isParent Component
local IsParentComponent = ComponentType()
IsParentComponent.Name = "IsParent"
IsParentComponent.TableType = TableType.Map
worldCreator:AddComponentType(IsParentComponent)

-- BoundToEntity Component
local BoundToEntityComponent = ComponentType()
BoundToEntityComponent.Name = "BoundToEntity"
BoundToEntityComponent.TableType = TableType.Map
BoundToEntityComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(BoundToEntityComponent)

-- WorldToViewSpace Component
local WorldToViewSpaceComponent = ComponentType()
WorldToViewSpaceComponent.Name = "WorldToViewSpace"
WorldToViewSpaceComponent.SyncNetwork = true
WorldToViewSpaceComponent.TableType = TableType.Map
worldCreator:AddComponentType(WorldToViewSpaceComponent)

-- Spin Component
local SpinComponent = ComponentType()
SpinComponent.Name = "Spin"
SpinComponent.SyncNetwork = true
SpinComponent.TableType = TableType.Map
SpinComponent:AddVariable("X", ByteSize.Float)
SpinComponent:AddVariable("Y", ByteSize.Float)
SpinComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(SpinComponent)