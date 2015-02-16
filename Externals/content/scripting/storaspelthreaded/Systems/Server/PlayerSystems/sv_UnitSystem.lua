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

UnitSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
	local mySeed = os.time() - 1418742000 -- dont ask
	math.randomseed(mySeed)
	for n = 1, #entities do
		local entity = entities[n]
		
		print("unit entity added")
	
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
			
			local r = math.random()
			local g = math.random()
			local b = math.random()
			world:GetComponent(newEntityId, "Color", "X"):SetFloat3(r, g, b)
			
			world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
			world:SetComponent(newEntityId, "PlayerEntityId", "Id", entity)
			world:SetComponent(newEntityId, "TargetCheckpoint", "Id", 1)
			world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)
			world:CreateComponentAndAddTo("NeedSpawnLocation", newEntityId)
			
			world:SetComponent(entity, "PlayerNumber", "Number", playerNumber)
			world:CreateComponentAndAddTo("UnitEntityId", entity)
			world:SetComponent(entity, "UnitEntityId", "Id", newEntityId)
			
			if world:EntityHasComponent(entity, "NetConnection") then
				local ip = world:GetComponent(entity, "NetConnection", "IpAddress"):GetText()
				local port = world:GetComponent(entity, "NetConnection", "Port"):GetInt()
    
				local id = Net.StartPack("Client.SendPlayerUnitId")
				Net.WriteInt(id, playerNumber)
				Net.Send(id, ip, port)
			end
						
		elseif world:EntityHasComponent(entity, "RemoveUnit") then
			local plyNum = world:GetComponent(entity, "RemoveUnit", "PlayerNo"):GetInt()
			self.FreeSlots[#self.FreeSlots + 1] = plyNum
			--table.insert(self.FreeSlots, plyNum)
			local unitId = world:GetComponent(entity, "RemoveUnit", "UnitEntityId"):GetInt()
			world:KillEntity(unitId)
			world:KillEntity(entity)
		end
		
	end
end