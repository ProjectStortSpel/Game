--	Player template [TODO: Add shit]
local AI = EntityTemplate()
AI.Name = "AI"
AI:AddComponent("Player")
AI:AddComponent("SyncNetwork")			--	Sync this entity
EntityTemplateManager:AddTemplate(AI)	--	Add the template

--	Player template
local PlayerTemplate = EntityTemplate()
PlayerTemplate.Name = "Player"
PlayerTemplate:AddComponent("Player")
PlayerTemplate:AddComponent("PlayerNumber")
PlayerTemplate:AddComponent("PlayerName")
PlayerTemplate:AddComponent("UnitEntityId")
PlayerTemplate:SetString("NetConnection", "0.0.0.0", "IpAddress")
PlayerTemplate:SetInt("NetConnection", 0, "Port")
PlayerTemplate:AddComponent("SyncNetwork")			--	Sync this entity
EntityTemplateManager:AddTemplate(PlayerTemplate)	--	Add the template


--	Unit template
local Unit = EntityTemplate()
Unit.Name = "Unit"

	Unit:AddComponent("Unit")
	Unit:AddComponent("Model")

	--	Model data
	Unit:SetFloat3("Position", 0.0, 1.0, 0.0)
	Unit:SetFloat3("Rotation", 0.0, math.pi, 0.0)
	Unit:SetFloat3("Scale", 0.5, 0.5, 0.5)
	
	--	Game Logic
	Unit:AddComponent("PlayerNumber")			--	The number of the player [1, Number of players]
	Unit:AddComponent("Direction")
	Unit:AddComponent("TargetCheckpoint")
	Unit:AddComponent("MapPosition")
	Unit:AddComponent("Spawnpoint")
	
	
	--	Misc
	Unit:AddComponent("PlayerEntityId")			--	Entity ID to the specific player entity that controls this unit
	Unit:AddComponent("SyncNetwork")			--	Sync this entity
	
EntityTemplateManager:AddTemplate(Unit)		--	Add the template