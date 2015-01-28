CheckpointSystem = System()

CheckpointSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CheckpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckCheckpoint", FilterType.RequiresOneOf)
end

CheckpointSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		-- If the entity has a CheckCheckpoint component
		if world:EntityHasComponent( entity, "CheckCheckpoint") then

			-- Get all units currently on the playfield
			local units = self:GetEntities("Unit")
			-- Get all checkpoints on the map
			local checkPoints = self:GetEntities("Checkpoint")
			
			for i = 1, #units do
			
				-- Get the id of the unit's next checkpoint
				local targetId = world:GetComponent(units[i], "TargetCheckpoint", "Id"):GetInt()
				
				if targetId > #checkPoints then
				
					local newId = world:CreateNewEntity()
					world:CreateComponentAndAddTo("CheckFinishpoint", newId)
					world:CreateComponentAndAddTo("UnitEntityId", newId)
					world:SetComponent(newId, "UnitEntityId", "Id", units[i])
				
				else
				
					-- Get the unit's MapPosition
					local unitPosX, unitPosZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
				
					-- Go through all checksoints
					for j = 1, #checkPoints do
					
						-- Get the current checkpoints Id
						local cpId = world:GetComponent(checkPoints[j], "Checkpoint", "Number"):GetInt()
						
						-- If the id is the same as the unit's target checkpoint
						if cpId == targetId then
							
							-- Get the checkpoint's MapPosition
							local cpPosX, cpPosZ = world:GetComponent(checkPoints[j], "MapPosition", 0):GetInt2()
							
							-- If the unit's position and the checkpoint's position is the same
							if unitPosX == cpPosX and unitPosZ == cpPosZ then
							
								-- Get the number of the player controlling the unit
								local playerNum = world:GetComponent(units[i], "PlayerNumber", 0):GetInt()

								
								
								local totemPieceId = world:CreateNewEntity()
								world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
								world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
								world:CreateComponentAndAddTo("CheckpointId", totemPieceId)
								world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNum)

								world:SetComponent(totemPieceId, "CheckpointId", "Id", cpId)
								
								-- Set the new target checkpoint
								world:SetComponent(units[i], "TargetCheckpoint", "Id", targetId + 1)
								-- Set the units spawnpoint
								world:GetComponent(units[i], "Spawnpoint", 0):SetInt2(cpPosX, cpPosZ)
							
								break
							end
						end
					end
				end
			end
			world:KillEntity(entity)
		end
	end

end
