FinishpointSystem = System()

FinishpointSystem.Initialize = function(self)
	--	Set Name
	self:SetName("FinishpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Finishpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckFinishpoint", FilterType.RequiresOneOf)
end

FinishpointSystem.CreateTotemPiece = function(self, playerNumber, finishPoint)

	local totemPieceId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
	world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
	world:CreateComponentAndAddTo("CheckpointId", totemPieceId)
	
	world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNumber)
	world:SetComponent(totemPieceId, "CheckpointId", "Id", finishPoint)

end


FinishpointSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "CheckFinishpoint") then 
		
			-- Get all units currently on the playfield
			local units = self:GetEntities("Unit")
			
			-- Get all finishpoints on the map
			local finishPoints = self:GetEntities("Finishpoint")
			
			for i = 1, #finishPoints do
			
				-- Get the unit's id to check for
				local unitId = world:GetComponent(entity, "UnitEntityId", 0):GetInt()
				
				-- Get the position of the finishPoints[i]
				local finishPosX, finishPosZ = world:GetComponent(finishPoints[i], "MapPosition", 0):GetInt2()
			
				-- Go through all units currently active
				for j = 1, #units do
					
					-- If the unitId to check for equals the current unit
					if unitId == units[j] then
					
						-- Get the units MapPosition, Number of steps to check & the units direction
						local unitPosX, unitPosZ = world:GetComponent(units[j], "MapPosition", 0):GetInt2()
						local noSteps = world:GetComponent(units[j], "NoSubSteps", 0):GetInt()
						local dirX, dirZ = world:GetComponent(units[j], "Direction", 0):GetInt2()
						
						-- Normalize the direction
						if dirX ~= 0 then
							dirX = dirX / math.abs(dirX)
						end
						if dirZ ~= 0 then
							dirZ = dirZ / math.abs(dirZ)
						end
						
						-- Loop through all substeps
						for tmp = noSteps - 1, 0, -1 do
							local tmpX = unitPosX - (dirX * tmp)
							local tmpZ = unitPosZ - (dirZ * tmp)
							
							if tmpX == finishPosX and tmpZ == finishPosZ then
								local playerId = world:GetComponent(units[j], "PlayerEntityId", "Id"):GetInt()
								local playerNum = world:GetComponent(units[j], "PlayerNumber", 0):GetInt()
								
								-- Create a new totem piece
								self:CreateTotemPiece(playerNum, finishPoints[i])
								
								-- If the unit is not controlled by a AI
								if not world:EntityHasComponent(playerId, "AI") then
								
									-- Take all cards from the player
									local newId = world:CreateNewEntity()
									world:CreateComponentAndAddTo("TakeCardsFromPlayer", newId)
									world:GetComponent(newId, "TakeCardsFromPlayer", "Player"):SetInt(playerId)
									
									-- Take all steps from the player
									newId = world:CreateNewEntity()
									world:CreateComponentAndAddTo("TakeCardStepsFromUnit", newId)
									world:GetComponent(newId, "TakeCardStepsFromUnit", "Unit"):SetInt(units[j])
								else
									-- Else if the player is an AI, remove it when done
									world:KillEntity(playerId)
								end
								
								-- Remove the unit
								world:KillEntity(units[j])
								
							end
							
						end
						
						
						
						
					end
					
					
					
				
				end
			
			end
		
			-- Remove the CheckFinishpoint entity
			world:KillEntity(entity)
		
		end
	end
end