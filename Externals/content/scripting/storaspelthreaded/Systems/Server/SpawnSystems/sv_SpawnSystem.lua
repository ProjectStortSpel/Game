SpawnSystem = System()

SpawnSystem.Initialize = function(self)
	--	Set Name
	self:SetName("SpawnSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Spawn", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Spawnpoint", FilterType.Mandatory)
end

SpawnSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
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
end