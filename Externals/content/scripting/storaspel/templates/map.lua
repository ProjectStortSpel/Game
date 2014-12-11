local Tile = EntityTemplate()
Tile.Name = "Tile"
Tile:SetFloat3("Position", 0.0, 0.0, 0.0)
Tile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Tile:SetFloat3("Scale", 1.0, 1.0, 1.0)
--Tile:SetModel("Model", "grass", "grass")
EntityTemplateManager:AddTemplate(Tile)

local Player = EntityTemplate()
Player.Name = "Player"
Player:SetFloat3("Position", 0.0, 1.0, 0.0)
Player:SetFloat3("Rotation", 0.0, math.pi, 0.0)
Player:SetFloat3("Scale", 0.5, 0.5, 0.5)
Player:SetModel("Model", "head", "head")
Player:SetInt2("Direction", 0, -1)
EntityTemplateManager:AddTemplate(Player)