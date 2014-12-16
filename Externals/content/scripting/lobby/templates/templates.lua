
local Head = EntityTemplate()
Head.Name = "Head"
Head:SetFloat3("Position", 0.0, 0.0, 0.0)
Head:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Head:SetFloat3("Scale", 1.0, 1.0, 1.0)
Head:SetModel("Model", "head", "head")
EntityTemplateManager:AddTemplate(Head)