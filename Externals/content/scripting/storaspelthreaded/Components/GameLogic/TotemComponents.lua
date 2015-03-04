--	This file will contain
--	components that are needed
--	for Totem Poles

-- TotemPole Component
local NewComponent = ComponentType()
NewComponent.Name = "TotemPole"
NewComponent:AddVariable("Height", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "AddTotemPiece"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "TotemPiece"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("TotemPoleId", ByteSize.Int)
NewComponent:AddVariable("CurrentHeight", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)