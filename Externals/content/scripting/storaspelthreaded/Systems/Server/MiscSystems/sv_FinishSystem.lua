FinishSystem = System()

FinishSystem.Initialize = function(self)
	--	Set Name
	self:SetName("FinishSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Finishpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckFinishpoint", FilterType.RequiresOneOf)
end

FinishSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

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
					
						-- Get the units MapPosition
						local unitPosX, unitPosZ = world:GetComponent(units[j], "MapPosition", 0):GetInt2()
						
						-- If the unit is at the same position as the finishpoint
						if unitPosX == finishPosX and unitPosZ == finishPosZ then
						
							-- Get the player id and number that is controlling the unit
							local playerId = world:GetComponent(units[j], "PlayerEntityId", "Id"):GetInt()
							local playerNum = world:GetComponent(units[j], "PlayerNumber", 0):GetInt()
							print("Player#" .. playerNum .. " has reached the finishpoint!")
							
							-- Add a new piece to the totempole at the finishpoint
							--self.AddTotemPole(self, playerNum, i, 0, finishPosX, 1, finishPosZ)
							
							
							-- Create a new entity which will take the player's cards
							local newId = world:CreateNewEntity()
							world:CreateComponentAndAddTo("TakeCardsFromPlayer", newId)
							world:GetComponent(newId, "TakeCardsFromPlayer", "Player"):SetInt(playerId)
							
							-- Create a new entity which will take the player's CardSteps
							newId = world:CreateNewEntity()
							world:CreateComponentAndAddTo("TakeCardStepsFromUnit", newId)
							world:GetComponent(newId, "TakeCardStepsFromUnit", "Unit"):SetInt(units[j])
							
							-- Add a spectator to the current player,
							-- which will remove the player from the game and add him as a spectator
							world:CreateComponentAndAddTo("IsSpectator", playerId)
							
							
							print("finishPoints[i]: " .. finishPoints[i])
							
							local totemPieceId = world:CreateNewEntity()
							world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
							world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
							world:CreateComponentAndAddTo("CheckpointId", totemPieceId)
							print("CreateComponentAndAddTo done")
							world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNum)
							print("PlayerNumber done")
							world:SetComponent(totemPieceId, "CheckpointId", "Id", finishPoints[i])
							print("CheckpointId done")
							
							
							
							-- Kill the unit entity
							--world:KillEntity(units[j])
							
							
						end
						
						
					end
					
					
					
				
				end
			
			end
		
			-- Remove the CheckFinishpoint entity
			world:KillEntity(entity)
		
		end
	end
end