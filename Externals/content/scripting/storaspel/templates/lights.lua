
local PointlightRed = EntityTemplate()
PointlightRed.Name = "Pointlight"
PointlightRed:SetPointLight("Pointlight", 7.0, 4.0, 7.0, 1.0, 0.8, 0.8, 0.8, 0.2, 0.2, 10.0)
PointlightRed:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(PointlightRed)