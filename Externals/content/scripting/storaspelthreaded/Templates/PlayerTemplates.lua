--	AI template
local AITemplate = EntityTemplate()
AITemplate.Name = "AI"
--AITemplate:AddComponent("Player")
AITemplate:AddComponent("AI")
AITemplate:AddComponent("PlayerNumber")
AITemplate:AddComponent("PlayerName")
--AITemplate:AddComponent("UnitEntityId")
AITemplate:AddComponent("PickingDone")
EntityTemplateManager:AddTemplate(AITemplate)	--	Add the template

--	Player template
local PlayerTemplate = EntityTemplate()
PlayerTemplate.Name = "Player"
PlayerTemplate:AddComponent("Player")
PlayerTemplate:AddComponent("PlayerNumber")
PlayerTemplate:AddComponent("PlayerName")
--PlayerTemplate:AddComponent("UnitEntityId")
PlayerTemplate:AddComponent("PickingDone")
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
	Unit:SetFloat3("Position", -1337, -1337, -1337)
	Unit:SetFloat3("Rotation", 0.0, math.pi, 0.0)
	Unit:SetFloat3("Scale", 0.8, 0.8, 0.8)
	
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

--	Ghost template
local Ghost = EntityTemplate()
Ghost.Name = "Ghost"

	Ghost:AddComponent("Model")

	--	Model data
	Ghost:SetFloat3("Position", -1337, -1337, -1337)
	Ghost:SetFloat3("Rotation", 0.0, math.pi, 0.0)
	Ghost:SetFloat3("Scale", 0.8, 0.8, 0.8)
	Ghost:SetFloat3("Color", 0.2, 0.2, 0.2)
	
	--	Game Logic
	Ghost:AddComponent("Direction")
	Ghost:AddComponent("MapPosition")
	
EntityTemplateManager:AddTemplate(Ghost)		--	Add the template