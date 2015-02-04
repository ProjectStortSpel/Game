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

CheckpointSystem.AddTotemPiece = function(self, playerNumber, checkpoint)

	local totemPieceId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
	world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
	world:CreateComponentAndAddTo("CheckpointId", totemPieceId)

	world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNumber)
	world:SetComponent(totemPieceId, "CheckpointId", "Id", checkpoint)
	
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
				
				-- If the target is larger then the number of checkpoints it is a finishpoint
				if targetId > #checkPoints then
					print("Check finish!")
					-- Check finishpoints
					local newId = world:CreateNewEntity()
					world:CreateComponentAndAddTo("CheckFinishpoint", newId)
					world:CreateComponentAndAddTo("UnitEntityId", newId)
					world:SetComponent(newId, "UnitEntityId", "Id", units[i])
				
				else
				
					-- Get the number of substeps
					local noSteps = world:GetComponent(units[i], "NoSubSteps", 0):GetInt2()
					local dirX, dirZ = world:GetComponent(units[i], "Direction", 0):GetInt2()
					
					-- Get the units' MapPosition
					local unitPosX, unitPosZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
					
					for tmp = noSteps - 1, 0, -1 do
						
						local tmpX = unitPosX - (dirX * tmp)
						local tmpZ = unitPosZ - (dirZ * tmp)
						
						for j = 1, #checkPoints do
						
							-- Get the current checkpoints Id
							local cpId = world:GetComponent(checkPoints[j], "Checkpoint", "Number"):GetInt()
							
							-- If the id is the same as the units' target checkpoint
							if cpId == targetId then
								
								-- Get the checkpoints' MapPosition
								local cpPosX, cpPosZ = world:GetComponent(checkPoints[j], "MapPosition", 0):GetInt2()
								
								-- If the units' position and the checkpoints' position is the same
								if tmpX == cpPosX and tmpZ == cpPosZ then
									-- Get the number of the player controlling the unit
									local playerNum = world:GetComponent(units[i], "PlayerNumber", 0):GetInt()
									-- Add a new totempiece
									self:AddTotemPiece(playerNum, cpId)
									
									-- Change the players target checkpoint to the next one
									world:SetComponent(units[i], "TargetCheckpoint", "Id", targetId + 1)
									-- Set the players spawnpoint to this checkpoint
									world:GetComponent(units[i], "Spawnpoint", 0):SetInt2(cpPosX, cpPosZ)
									break
								end
								
							end
						
						end
						
					end
					
				
				end
			end
			
			world:KillEntity(entity)
		end
	end

end
