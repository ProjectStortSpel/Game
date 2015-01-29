--	This file will contain
--	components that are needed
--	for Totem Poles

-- TotemPole Component
local TotemPoleComponent = ComponentType()
TotemPoleComponent.Name = "TotemPole"
TotemPoleComponent:AddVariable("Height", ByteSize.Int)
TotemPoleComponent.TableType = TableType.Map
worldCreator:AddComponentType(TotemPoleComponent)

local AddTotemPieceComponent = ComponentType()
AddTotemPieceComponent.Name = "AddTotemPiece"
AddTotemPieceComponent.TableType = TableType.Map
worldCreator:AddComponentType(AddTotemPieceComponent)