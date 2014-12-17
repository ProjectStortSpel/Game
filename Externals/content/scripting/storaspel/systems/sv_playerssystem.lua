PlayersSystem = System()

PlayersSystem.Update = function(self, dt)

end

PlayersSystem.Initialize = function(self)
	self:SetName("Players Connected System")
	
	self:AddComponentTypeToFilter("Player", FilterType.Mandatory)
end

PlayersSystem.OnEntityAdded = function(self, entityId)
	print("Ny spelare!")
	
	
	local playerComp = self:GetComponent(entityId, "Player", "PlayerNumber")
	local playerNumber = 1
	local newEntityId = world:CreateNewEntity("Unit")
	
	world:SetComponent(newEntityId, "Model", "ModelName", "head");
	world:SetComponent(newEntityId, "Model", "ModelPath", "head");
	world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
	
	print("Unit ", newEntityId)
end

PlayersSystem.OnEntityRemoved = function(self, entityId)
	print("Rip spelare")
end