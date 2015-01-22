local Tile = EntityTemplate()
Tile.Name = "Tile"
Tile:SetFloat3("Position", 0.0, 0.0, 0.0)
Tile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Tile:SetFloat3("Scale", 1.0, 1.0, 1.0)
Tile:AddComponent("MapPosition")
Tile:AddComponent("TileComp")
Tile:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Tile)

local Card = EntityTemplate()
Card.Name = "Card"
Card:AddComponent("Card")
Card:AddComponent("CardPrio")
Card:AddComponent("CardAction")
EntityTemplateManager:AddTemplate(Card)