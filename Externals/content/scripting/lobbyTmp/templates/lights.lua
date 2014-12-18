
local PointlightRed = EntityTemplate()
PointlightRed.Name = "Pointlight"
PointlightRed:SetPointlight("Pointlight", 7.0, 4.0, 7.0, 1.0, 0.8, 0.8, 0.8, 0.2, 0.2, 10.0)
PointlightRed:SetFloat5("RotateAround", 7.0, 4.0, 7.0, 4.0, 5.0)
PointlightRed:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(PointlightRed)