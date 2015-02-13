AbilityIWin = System()

AbilityIWin.Initialize = function(self)

	self:SetName("AbilityIWin")
		
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("UnitIWin", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
end

AbilityIWin.VisitCheckpoint = function(self, unit, checkpointNumber)

	local	nCheckpoints = self:GetEntities("Checkpoint")
	for checkpointId = 1, #nCheckpoints do
		local tempCheckpoint	=	world:GetComponent(nCheckpoints[checkpointId], "Checkpoint", "Number"):GetInt()
		
		if checkpointNumber == tempCheckpoint then
			local mapX, mapZ = world:GetComponent(nCheckpoints[checkpointId], "MapPosition", "X"):GetInt2()
			local newCheck = world:CreateNewEntity()
			world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
			world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(unit)
			world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(mapX)
			world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(mapZ)
		end
	end
end

AbilityIWin.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "UnitIWin") then
			local	startId = world:GetComponent(entity, "TargetCheckpoint", "Id"):GetInt()
			local	nCheckpoints = self:GetEntities("Checkpoint")
			
			for n = 1, #nCheckpoints do
				self:VisitCheckpoint(entity, n)
			end
			world:RemoveComponentFrom("UnitIWin", entity)
		end
	end
end