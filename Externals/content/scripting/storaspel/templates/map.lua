local Tile = EntityTemplate()
Tile.Name = "Tile"
Tile:SetFloat3("Position", 0.0, 0.0, 0.0)
Tile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Tile:SetFloat3("Scale", 1.0, 1.0, 1.0)
Tile:AddComponent("SyncNetwork")
--Tile:SetModel("Model", "grass", "grass")
EntityTemplateManager:AddTemplate(Tile)

local Card = EntityTemplate()
Card.Name = "Card"
Card:AddComponent("CardPrio")
Card:AddComponent("CardAction")
Card:SetFloat3("Position", 0.0, 0.0, 0.0)
Card:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Card:SetFloat3("Scale", 1.0, 1.0, 1.0)
Card:SetModel("Model", "forward", "cards")
EntityTemplateManager:AddTemplate(Card)