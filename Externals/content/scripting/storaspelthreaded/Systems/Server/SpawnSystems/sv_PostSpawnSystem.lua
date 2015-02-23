PostSpawnSystem = System()

PostSpawnSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PostSpawnSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Unit",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PostSpawn",FilterType.RequiresOneOf)
end

PostSpawnSystem.CheckAvailableSpawn = function(self, unitToTest, allUnits)

	for i = 1, #allUnits do
		
		if unitToTest ~= allUnits[i] then
		
		
		end
	end
end

PostSpawnSystem.EntitiesAdded = function(self, dt, newSpawns)

	for n = 1, #newSpawns do
		local entityId = newSpawns[n]
		
		if world:EntityHasComponent(entityId, "PostSpawn") then
		
			printf("asdasd")
			
			
			world:KillEntity(entityId)
		end
	end
	
end