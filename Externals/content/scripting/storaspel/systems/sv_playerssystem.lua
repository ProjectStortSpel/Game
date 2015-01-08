PlayersSystem = System()
PlayersSystem.NextSlot = 1
PlayersSystem.FreeSlots = {}

PlayersSystem.Update = function(self, dt)

end

PlayersSystem.Initialize = function(self)
	self:SetName("Players Connected System")
	
	self:AddComponentTypeToFilter("Player", FilterType.Mandatory)
	self:AddComponentTypeToFilter("ActiveNetConnection", FilterType.Mandatory)

	print("Players Connected System initialized!")
end

PlayersSystem.OnEntityAdded = function(self, entityId)
	print("Ny spelare!")
	
	local playerNumber
	if #PlayersSystem.FreeSlots ~= 0 then
		playerNumber = PlayersSystem.FreeSlots[1]
		table.remove(PlayersSystem.FreeSlots, 1)
	else
		playerNumber = PlayersSystem.NextSlot
		PlayersSystem.NextSlot = PlayersSystem.NextSlot + 1
	end

	local newEntityId = world:CreateNewEntity("Unit")

	world:SetComponent(newEntityId, "Model", "ModelName", "head");
	world:SetComponent(newEntityId, "Model", "ModelPath", "head");

	world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
	world:SetComponent(newEntityId, "PlayerEntityId", "Id", entityId)
	world:SetComponent(newEntityId, "TargetCheckpoint", "Id", 1)
	world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)



	world:SetComponent(entityId, "PlayerNumber", "Number", playerNumber)
	world:SetComponent(entityId, "UnitEntityId", "Id", newEntityId)


	print("Unit ", newEntityId)
end

PlayersSystem.OnEntityRemoved = function(self, entityId)
	print("Rip spelare")

	local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
	table.insert(PlayersSystem.FreeSlots, plyNum)

	local unitId = self:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()

	world:KillEntity(unitId)

end