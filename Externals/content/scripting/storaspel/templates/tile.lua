local Tile = EntityTemplate()
Tile.Name = "Tile"
Tile:SetFloat3("Position", 0.0, 0.0, 0.0)
Tile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Tile:SetFloat3("Scale", 1.0, 1.0, 1.0)
--Tile:SetModel("Model", "grass", "grass")
EntityTemplateManager:AddTemplate(Tile)

local RiverTile = EntityTemplate()
RiverTile.Name = "River"
RiverTile:SetFloat3("Position", 0.0, 0.0, 0.0)
RiverTile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
RiverTile:SetFloat3("Scale", 1.0, 1.0, 1.0)
RiverTile:SetModel("Model", "rivercorner", "rivercorner")
EntityTemplateManager:AddTemplate(RiverTile)