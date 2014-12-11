local Tile = EntityTemplate()
Tile.Name = "Tile"
Tile:SetFloat3("Position", 0.0, 0.0, 0.0)
Tile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Tile:SetFloat3("Scale", 1.0, 1.0, 1.0)
Tile:SetModel("Model", "cube", "cube")
EntityTemplateManager:AddTemplate(Tile)