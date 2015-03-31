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

-- HATS
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

-- PLAYERCOLORS
NewComponent = ComponentType()
NewComponent.Name = "PlayerColor"
NewComponent:AddVariable("Id", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "PrevColor"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "NextColor"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "ThisColor"
NewComponent:AddVariable("colorNr", ByteSize.Int)
NewComponent:AddVariable("unitId", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)



NewComponent = ComponentType()
NewComponent.Name = "WinScreen"
NewComponent:AddVariable("TimeToDelay", ByteSize.Float)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "DisplayPostMatch"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)
