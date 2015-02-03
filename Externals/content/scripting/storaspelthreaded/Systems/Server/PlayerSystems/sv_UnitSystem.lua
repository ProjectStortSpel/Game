UnitSystem = System()
UnitSystem.NextSlot = 1
UnitSystem.FreeSlots = {}

UnitSystem.Initialize = function(self)
	self:SetName("Unit System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("NeedUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("RemoveUnit", FilterType.RequiresOneOf)
	--self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
end

--UnitSystem.FindEmptyId = function(self)
--
--	local currentPlayers = self:GetEntities()
--	local newPlayerId = 5
--	
--	local availableIds = {}
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
	
	for i = 1, #entities do
		if world:EntityHasComponent(entities[i], "NeedUnit") then
			
			world:RemoveComponentFrom("NeedUnit", entities[i])
			
			local playerNumber
			if #self.FreeSlots ~= 0 then
				playerNumber = self.FreeSlots[1]
				table.remove(self.FreeSlots, 1)
			else
				playerNumber = self.NextSlot
				self.NextSlot = self.NextSlot + 1
			end
			
			local newEntityId = world:CreateNewEntity("Unit")
			
			world:SetComponent(newEntityId, "Model", "ModelName", "ply" .. playerNumber);
			world:SetComponent(newEntityId, "Model", "ModelPath", "caveman");
			world:SetComponent(newEntityId, "Model", "RenderType", 0);
			
			world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
			world:SetComponent(newEntityId, "PlayerEntityId", "Id", entities[i])
			world:SetComponent(newEntityId, "TargetCheckpoint", "Id", 1)
			world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)
			world:CreateComponentAndAddTo("NeedSpawnLocation", newEntityId)
			
			world:SetComponent(entities[i], "PlayerNumber", "Number", playerNumber)
			world:SetComponent(entities[i], "UnitEntityId", "Id", newEntityId)
			
			if world:EntityHasComponent(entities[i], "NetConnection") then
				
				local ip = world:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString()
				local port = world:GetComponent(entities[i], "NetConnection", "Port"):GetInt()

				local id = Net.StartPack("Client.SendPlayerUnitId")
				Net.WriteInt(id, playerNumber)
				Net.Send(id, ip, port)
			end
			
			print("unit add")
			
		elseif world:EntityHasComponent(entities[i], "RemoveUnit") then
			
			local plyNum = self:GetComponent(entities[i], "RemoveUnit", "PlayerNo"):GetInt()
			table.insert(self.FreeSlots, plyNum)
			local unitId = self:GetComponent(entities[i], "RemoveUnit", "UnitEntityId"):GetInt()
			world:KillEntity(unitId)
			world:KillEntity(entities[i])
			
			print("unit and player kill")
		end
	end
end