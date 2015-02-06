CheckpointSystem = System()

CheckpointSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CheckpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckCheckpointForEntity", FilterType.RequiresOneOf)
	
end

CheckpointSystem.AddTotemPiece = function(self, playerNumber, checkpoint)

	local totemPieceId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
	world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
	world:CreateComponentAndAddTo("CheckpointId", totemPieceId)

	world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNumber)
	world:SetComponent(totemPieceId, "CheckpointId", "Id", checkpoint)
	
end

CheckpointSystem.HasReachedFinish = function(self, entityId)
	
	local	playerId	= 	world:GetComponent(entityId, "PlayerEntityId", "Id"):GetInt()
	
	-- If the unit is not controlled by a AI
	if not world:EntityHasComponent(playerId, "AI") then
		-- Take all cards from the player
		local newId = world:CreateNewEntity()
		world:CreateComponentAndAddTo("TakeCardsFromPlayer", newId)
		world:GetComponent(newId, "TakeCardsFromPlayer", "Player"):SetInt(playerId)
		
		-- Take all steps from the player
		newId = world:CreateNewEntity()
		world:CreateComponentAndAddTo("TakeCardStepsFromUnit", newId)
		world:GetComponent(newId, "TakeCardStepsFromUnit", "Unit"):SetInt(entityId)
		
		--	Make the player a spectator
		world:CreateComponentAndAddTo("IsSpectator", playerId)
	else
		-- Else if the player is an AI, remove it when done
		world:KillEntity(playerId)
	end
end

CheckpointSystem.CheckCheckpoint = function(self, entityId, posX, posZ)
	
	local	allCheckpoints		=	self:GetEntities("Checkpoint")
	local	targetCheckpoint  	= 	world:GetComponent(entityId, "TargetCheckpoint", "Id"):GetInt()
	
	for i = 1, #allCheckpoints do
		
		local tempCheckpointId = world:GetComponent(allCheckpoints[i], "Checkpoint", "Number"):GetInt()
		if tempCheckpointId == targetCheckpoint then
			local	tempX, tempZ  	= 	world:GetComponent(allCheckpoints[i], "MapPosition", "X"):GetInt2()
			if tempX == posX and tempZ == posZ then
				world:GetComponent(entityId, "TargetCheckpoint", "Id"):SetInt(targetCheckpoint+1)
				world:GetComponent(entityId, "Spawnpoint", 0):SetInt2(tempX, tempZ)
				local playerNum = world:GetComponent(entityId, "PlayerNumber", 0):GetInt()
				self:AddTotemPiece(playerNum, targetCheckpoint)
				if targetCheckpoint+1 > #allCheckpoints then
					self:HasReachedFinish(entityId)
				end
				return
			end
		end
	end
	
	
end

CheckpointSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, newEntities)

	for n = 1, #newEntities do
		local entity = newEntities[n]
		
		--	Check if the entity has reached the checkpoint
		if world:EntityHasComponent(entity, "CheckCheckpointForEntity") then
			local	tEntityId	=	world:GetComponent(entity, "CheckCheckpointForEntity", "EntityId"):GetInt()
			local	tPosX		=	world:GetComponent(entity, "CheckCheckpointForEntity", "PosX"):GetInt()
			local	tPosZ		=	world:GetComponent(entity, "CheckCheckpointForEntity", "PosZ"):GetInt()
			
			self:CheckCheckpoint(tEntityId, tPosX, tPosZ)
			world:KillEntity(entity)
		end
		
	end
	
end