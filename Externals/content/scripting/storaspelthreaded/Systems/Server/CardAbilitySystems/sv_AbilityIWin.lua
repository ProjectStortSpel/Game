AbilityIWin = System()

AbilityIWin.Initialize = function(self)

	self:SetName("AbilityIWin")
		
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("UnitIWin", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
end

AbilityIWin.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "UnitIWin") then
			print("UnitIWin played!")
			local	startId = world:GetComponent(entity, "TargetCheckpoint", "Id"):GetInt()
			local	nCheckpoints = self:GetEntities("Checkpoint")
			
			print("Target: " .. startId)
			print("Checkpoints: " .. #nCheckpoints)
			
			for checkpointId = startId, #nCheckpoints do
				
				local mapX, mapZ = world:GetComponent(nCheckpoints[checkpointId], "MapPosition", "X"):GetInt2()
				print("Checkpoint #" .. checkpointId .. " at: " .. mapX .. ", " .. mapZ)
				local newCheck = world:CreateNewEntity()
				world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(entity)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(mapX)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(mapZ)
				
			end
			
			world:RemoveComponentFrom("UnitIWin", entity)
		end
	end
end