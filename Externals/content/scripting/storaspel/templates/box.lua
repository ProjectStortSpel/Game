
local Head = EntityTemplate()
Head.Name = "Head"
Head:SetFloat3("Position", 0.0, 0.0, 0.0)
Head:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Head:SetFloat3("Scale", 1.0, 1.0, 1.0)
Head:SetModel("Model", "head", "head")
EntityTemplateManager:AddTemplate(Head)

local Hole = EntityTemplate()
Hole.Name = "Hole"
Hole:SetFloat3("Position", 0.0, 0.0, 0.0)
Hole:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Hole:SetFloat3("Scale", 1.0, 1.0, 1.0)
Hole:SetModel("Model", "hole_test", "hole")
EntityTemplateManager:AddTemplate(Hole)

local GrassTile = EntityTemplate()
GrassTile.Name = "Grass"
GrassTile:SetFloat3("Position", 0.0, 0.0, 0.0)
GrassTile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
GrassTile:SetFloat3("Scale", 1.0, 1.0, 1.0)
GrassTile:SetModel("Model", "grass", "grass")
EntityTemplateManager:AddTemplate(GrassTile)