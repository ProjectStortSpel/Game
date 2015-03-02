--	Tornado template
local Tornado = EntityTemplate()
Tornado.Name = "Tornado"
Tornado:AddComponent("Model")
Tornado:SetFloat3("Position", 0.0, 1.0, 0.0)
Tornado:SetFloat3("Rotation", 0.0, math.pi, 0.0)
Tornado:SetFloat3("Scale", 0.0, 0.0, 0.0)
Tornado:SetFloat3("Spin", 0.0, 20.0, 0.0)
Tornado:AddComponent("Direction")
Tornado:AddComponent("MapPosition")
Tornado:AddComponent("SyncNetwork")			--	Sync this entity
	
EntityTemplateManager:AddTemplate(Tornado)	--	Add the template