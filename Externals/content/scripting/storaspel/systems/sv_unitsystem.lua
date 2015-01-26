UnitSystem = System()
UnitSystem.NextSlot = 1
UnitSystem.FreeSlots = {}

UnitSystem.Update = function(self, dt)

end

UnitSystem.Initialize = function(self)
	self:SetName("Unit System")
	
	self:AddComponentTypeToFilter("NeedUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("RemoveUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
end

UnitSystem.OnEntityAdded = function(self, entityId)
	
	if world:EntityHasComponent(entityId, "NeedUnit") then
	
		world:RemoveComponentFrom("NeedUnit", entityId)
		
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
		world:SetComponent(newEntityId, "PlayerEntityId", "Id", entityId)
		world:SetComponent(newEntityId, "TargetCheckpoint", "Id", 1)
		world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)
		world:CreateComponentAndAddTo("NeedSpawnLocation", newEntityId)
		
		world:SetComponent(entityId, "PlayerNumber", "Number", playerNumber)
		world:SetComponent(entityId, "UnitEntityId", "Id", newEntityId)
		
		if world:EntityHasComponent(entityId, "NetConnection") then
			
			local ip = world:GetComponent(entityId, "NetConnection", "IpAddress"):GetString()
			local port = world:GetComponent(entityId, "NetConnection", "Port"):GetInt()

			local id = Net.StartPack("Client.SendPlayerUnitId")
			Net.WriteInt(id, playerNumber)
			Net.Send(id, ip, port)
		end
		
	elseif world:EntityHasComponent(entityId, "RemoveUnit") then
		
		world:RemoveComponentFrom("RemoveUnit", entityId)
		local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
		table.insert(self.FreeSlots, plyNum)
		local unitId = self:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()
		world:KillEntity(unitId)
	end
end

UnitSystem.OnEntityRemoved = function(self, entityId)
	--print("Rip spelare")
    --
	--local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
	--table.insert(self.FreeSlots, plyNum)
    --
	--local unitId = self:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()
    --
	--world:KillEntity(unitId)

end