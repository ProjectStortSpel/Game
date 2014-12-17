
--	Player template [TODO: Add shit]
local AI = EntityTemplate()
AI.Name = "AI"
AI:AddComponent("Player")
AI:AddComponent("SyncNetwork")			--	Sync this entity
EntityTemplateManager:AddTemplate(AI)	--	Add the template

--	Player template
local Player = EntityTemplate()
Player.Name = "Player"
Player:AddComponent("Player")
Player:SetString("NetConnection", "0.0.0.0", "IpAddress")
Player:SetInt("NetConnection", 0, "Port")
Player:SetBool("NetConnection", false, "Active")
--Player:AddComponent("SyncNetwork")			--	Sync this entity
EntityTemplateManager:AddTemplate(Player)	--	Add the template


--	Unit template
local Unit = EntityTemplate()
Unit.Name = "Unit"

	--	Model data
	Unit:SetFloat3("Position", 0.0, 1.0, 0.0)
	Unit:SetFloat3("Rotation", 0.0, math.pi, 0.0)
	Unit:SetFloat3("Scale", 0.5, 0.5, 0.5)
	Unit:SetModel("Model", "head", "head")
	
	--	Game Logic
	Unit:AddComponent("PlayerNumber")			--	The number of the player [1, Number of players]
	Unit:AddComponent("Direction")
	Unit:AddComponent("TargetCheckpoint")
	Unit:AddComponent("MapPosition")
	
	
	--	Misc
	Unit:AddComponent("PlayerEntityId")			--	Entity ID to the specific player entity that controls this unit
	Unit:AddComponent("SyncNetwork")			--	Sync this entity
	
EntityTemplateManager:AddTemplate(Unit)		--	Add the template