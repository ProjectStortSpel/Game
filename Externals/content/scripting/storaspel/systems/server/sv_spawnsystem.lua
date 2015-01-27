SpawnSystem = System()

SpawnSystem.Update = function(self, dt)

end

SpawnSystem.Initialize = function(self)
	self:SetName("SpawnSystem")
	self:AddComponentTypeToFilter("Spawn", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Spawnpoint", FilterType.Mandatory)
end


SpawnSystem.OnEntityAdded = function(self, entityId)
	
	local posComp = self:GetComponent(entityId, "Position", 0)
	local mapPosComp = self:GetComponent(entityId, "MapPosition", 0)
	local spawnpointComp = self:GetComponent(entityId, "Spawnpoint", 0)

	local X, Z = spawnpointComp:GetInt2()

    posComp:SetFloat3(X, 0.5, Z)
	mapPosComp:SetInt2(X, Z)

	if world:EntityHasComponent(entityId, "Hide") then
		world:RemoveComponentFrom("Hide", entityId)
	end

	world:RemoveComponentFrom("Spawn", entityId)
	print("Spawned at " .. X .. " - " .. Z)

end

SpawnSystem.OnEntityRemoved = function(self, entityId)
	
end