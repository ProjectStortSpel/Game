CreateSpawnpointSystem = System()

CreateSpawnpointSystem.Update = function(self, dt)

end

CreateSpawnpointSystem.Initialize = function(self)
	self:SetName("CreateSpawnpointSystem")
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)
	self:AddComponentTypeToFilter("PlayerNumber", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Spawnpoint", FilterType.Mandatory)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

CreateSpawnpointSystem.PostInitialize = function(self)

end

CreateSpawnpointSystem.OnEntityAdded = function(self, entityId)

	local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number").GetInt()
	local mapPos = {2 + plyNum * 2, 12}	
	local comp = self:GetComponent(entityId, "Spawnpoint", 0)
	comp:SetInt2(mapPos[1], mapPos[2])

	
	world:CreateComponentAndAddTo("Spawn", entityId)

end

CreateSpawnpointSystem.OnEntityRemoved = function(self, entityId)
	
end