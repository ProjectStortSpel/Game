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