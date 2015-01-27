--	This file will contain
--	miscellaneous components

-- Special Special Special component
local NullComponent = ComponentType()
NullComponent.Name = "Null"
NullComponent.SyncNetwork = true
NullComponent.TableType = TableType.Map
worldCreator:AddComponentType(NullComponent)