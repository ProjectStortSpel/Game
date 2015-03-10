--	Random components

--	IsTree Component
local NewComponent = ComponentType()
NewComponent.Name = "IsTree"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)


NewComponent = ComponentType()
NewComponent.Name = "ToggleTree"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "GlowCheckpoint"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Stage", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "GlowingCheckpoint"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Stage", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)


NewComponent = ComponentType()
NewComponent.Name = "Hat"
NewComponent:AddVariable("Id", ByteSize.Int)
NewComponent:AddVariable("hatId", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "PrevHat"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "NextHat"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "ThisHat"
NewComponent:AddVariable("hatId", ByteSize.Int)
NewComponent:AddVariable("unitId", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)