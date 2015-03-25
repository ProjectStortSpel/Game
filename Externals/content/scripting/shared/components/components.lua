-- Null Component
local NullComponent = ComponentType()
NullComponent.Name = "Null"
NullComponent.SyncNetwork = true
NullComponent.TableType = TableType.Map
worldCreator:AddComponentType(NullComponent)

NullComponent = ComponentType()
NullComponent.Name = "AddedToRenderer"
NullComponent.TableType = TableType.Map
worldCreator:AddComponentType(NullComponent)

-- Parent Component
local ParentComponent = ComponentType()
ParentComponent.Name = "Parent"
ParentComponent.TableType = TableType.Map
ParentComponent.SyncNetwork = true
ParentComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(ParentComponent)
-- Parent Component
local ParentJointComponent = ComponentType()
ParentJointComponent.Name = "ParentJoint"
ParentJointComponent.TableType = TableType.Map
ParentJointComponent.SyncNetwork = true
ParentJointComponent:AddVariable("JointId", ByteSize.Int)
worldCreator:AddComponentType(ParentJointComponent)
-- isParent Component
local IsParentComponent = ComponentType()
IsParentComponent.Name = "IsParent"
IsParentComponent.TableType = TableType.Map
worldCreator:AddComponentType(IsParentComponent)
-- KillWhenOrphan Component
local KillWhenOrphanComponent = ComponentType()
KillWhenOrphanComponent.Name = "KillWhenOrphan"
KillWhenOrphanComponent.TableType = TableType.Map
worldCreator:AddComponentType(KillWhenOrphanComponent)

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

-- GenerateIsland Component
local GenerateIslandComponent = ComponentType()
GenerateIslandComponent.Name = "GenerateIsland"
GenerateIslandComponent.SyncNetwork = true
GenerateIslandComponent.TableType = TableType.Array
GenerateIslandComponent:AddVariable("Map", ByteSize.String)
GenerateIslandComponent:AddVariable("OffsetX", ByteSize.Int)
GenerateIslandComponent:AddVariable("OffsetZ", ByteSize.Int)
GenerateIslandComponent:AddVariable("SizeX", ByteSize.Int)
GenerateIslandComponent:AddVariable("SizeZ", ByteSize.Int)
worldCreator:AddComponentType(GenerateIslandComponent)

local HoverSound = ComponentType()
HoverSound.Name = "HoverSound"
HoverSound.TableType = TableType.Map
HoverSound:AddVariable("Sound", ByteSize.Text)
worldCreator:AddComponentType(HoverSound)