UnitSystem = System()
UnitSystem.NextSlot = 1
UnitSystem.FreeSlots = {}
UnitSystem.FreeSlots.__mode = "k"

UnitSystem.Initialize = function(self)
	self:SetName("UnitSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("NeedUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("RemoveUnit", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
end

--UnitSystem.FindEmptyId = function(self)
--
--	local currentPlayers = self:GetEntities()
--	local newPlayerId = 5
--	
--	local availableIds = {}
--	availableIds.__mode = "k"
--	for n = 1, 5 do
--		availableIds[n] = false
--	end
--	
--	for i = 1, #currentPlayers do
--		local playerId = world:GetComponent(currentPlayers[i], "PlayerNumber", "Number"):GetInt()
--		
--		if playerId ~= -1 then
--			availableIds[playerId] = true
--		end
--	end
--	
--	local newIndex = 1
--	while availableIds[newIndex] do
--		newIndex = newIndex + 1
--	end
--	
--	return newIndex
--end

UnitSystem.EntitiesAdded = function(self, dt, entities)
	
	local mySeed = os.time() - 1418742000 -- dont ask
	math.randomseed(mySeed)
	for n = 1, #entities do
		local entity = entities[n]
	
		if world:EntityHasComponent(entity, "NeedUnit") then
			
			world:RemoveComponentFrom("NeedUnit", entity)
			local playerNumber = nil
			if #self.FreeSlots ~= 0 then
				playerNumber = self.FreeSlots[1]
				table.remove(self.FreeSlots, 1)
			else
				playerNumber = self.NextSlot
				self.NextSlot = self.NextSlot + 1
			end
			
			local newEntityId = world:CreateNewEntity("Unit")
			
			world:SetComponent(newEntityId, "Model", "ModelName", "caveman");
			world:SetComponent(newEntityId, "Model", "ModelPath", "caveman");
			world:SetComponent(newEntityId, "Model", "RenderType", 0);
						
			world:GetComponent(newEntityId, "PlayerColor", "Id"):SetInt(0)
			local colorRequest = world:CreateNewEntity()
			world:CreateComponentAndAddTo("ThisColor", colorRequest)
			world:GetComponent(colorRequest, "ThisColor", "colorNr"):SetInt(playerNumber)
			world:GetComponent(colorRequest, "ThisColor", "unitId"):SetInt(newEntityId)

			--	Send to start fire on next checkpoint
			local	targetCheckpoint	=	1
			local	nCheckpoints		=	self:GetEntities("Checkpoint")
			local	X, Z, checkpointId	=	0,0,0
			for n = 1, #nCheckpoints do
				
				if world:GetComponent(nCheckpoints[n], "Checkpoint", "Number"):GetInt() == targetCheckpoint then
					checkpointId	=	nCheckpoints[n]
					X, Z			=	world:GetComponent(checkpointId, "MapPosition", "X"):GetInt2()
					break
				end
				
			end
			
			world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
			world:SetComponent(newEntityId, "PlayerEntityId", "Id", entity)
			world:SetComponent(newEntityId, "TargetCheckpoint", "Id", targetCheckpoint)
			world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)
			world:CreateComponentAndAddTo("NeedSpawnLocation", newEntityId)
			--world:CreateComponentAndAddTo("Hide", newEntityId)
			
			world:SetComponent(entity, "PlayerNumber", "Number", playerNumber)
			world:CreateComponentAndAddTo("UnitEntityId", entity)
			world:SetComponent(entity, "UnitEntityId", "Id", newEntityId)
			
			world:CreateComponentAndAddTo("PlayerStats", newEntityId)
			world:GetComponent(newEntityId, "PlayerStats", "PlayerNumber"):SetInt(playerNumber)
			world:GetComponent(newEntityId, "PlayerStats", "CardsPlayed"):SetInt(0)
			world:GetComponent(newEntityId, "PlayerStats", "Deaths"):SetInt(0)
			world:GetComponent(newEntityId, "PlayerStats", "Place"):SetInt(0)
			world:GetComponent(newEntityId, "PlayerStats", "GoalCheckpoint"):SetInt(0)
			
			if world:EntityHasComponent(entity, "NetConnection") then
			  
				local ip = world:GetComponent(entity, "NetConnection", "IpAddress"):GetText()
				local port = world:GetComponent(entity, "NetConnection", "Port"):GetInt()
				local id = Net.StartPack("Client.SendMyUnitID")
				Net.WriteInt(id, newEntityId)
				Net.Send(id, ip, port)
				
				local audioId = Net.StartPack("Client.PlaySoundC")
				Net.WriteString(audioId, "Background")
				Net.WriteString(audioId, "Background")
				Net.WriteBool(audioId, true)
				Net.Send(audioId, ip, port)
			else
				-- CREATE TOTEM HAT ON AI
				local randomHat = math.random(0,100)
				local hatRequest = world:CreateNewEntity()
				world:CreateComponentAndAddTo("ThisHat", hatRequest)
				world:GetComponent(hatRequest, "ThisHat", "hatId"):SetInt(randomHat)
				world:GetComponent(hatRequest, "ThisHat", "unitId"):SetInt(newEntityId)
			end
						
		elseif world:EntityHasComponent(entity, "RemoveUnit") then
			local plyNum = world:GetComponent(entity, "RemoveUnit", "PlayerNo"):GetInt()
			self.FreeSlots[#self.FreeSlots + 1] = plyNum
			--table.insert(self.FreeSlots, plyNum)
			local unitId = world:GetComponent(entity, "RemoveUnit", "UnitEntityId"):GetInt()
			if world:EntityHasComponent(unitId, "LerpPosition") or world:EntityHasComponent(unitId, "LerpingPosition") then
				world:CreateComponentAndAddTo("KillAfterLerp", unitId)
			else
				world:KillEntity(unitId)
			end
			world:KillEntity(entity)
		end
		
	end
end