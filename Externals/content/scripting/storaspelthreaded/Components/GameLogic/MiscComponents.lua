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
