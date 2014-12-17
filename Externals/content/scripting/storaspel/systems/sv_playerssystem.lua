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
	
	--world:CreateComponentAndAddTo("Spawn", newEntityId) KRASH
	world:SetComponent(newEntityId, "Model", "ModelName", "head");
	world:SetComponent(newEntityId, "Model", "ModelPath", "head");
	world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
	
	--PlayerMovementSystem:SetPosition(newEntityId, 6.0, 1.0, 12.0)
	--local comp = self:GetComponent(entity, "Spawn", 0)
	--comp:SetInt2(6.0, 12.0)
	
	print("Unit ", newEntityId)
end

PlayersSystem.OnEntityRemoved = function(self, entityId)
	print("Rip spelare")
end