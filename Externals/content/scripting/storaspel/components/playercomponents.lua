
--	Player Component
local PlayerComponent = ComponentType()
PlayerComponent.Name = "Player"
PlayerComponent.TableType = TableType.Map
PlayerComponent:AddVariable("PlayerName", ByteSize.Text)
PlayerComponent:AddVariable("PlayerNumber", ByteSize.Int)
worldCreator:AddComponentType(PlayerComponent)

--	PlayerNumber Component
local PlayerNumberComponent = ComponentType()
PlayerNumberComponent.Name = "PlayerNumber"
PlayerNumberComponent.TableType = TableType.Map
PlayerNumberComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(PlayerNumberComponent)