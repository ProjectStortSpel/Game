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
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
end

CheckpointSystem.AddTotemPiece = function(self, playerNumber, checkpoint, colorX, colorY, colorZ)

	local totemPieceId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
	world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
	world:CreateComponentAndAddTo("CheckpointId", totemPieceId)
	world:CreateComponentAndAddTo("Color", totemPieceId)
	
	world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNumber)
	world:SetComponent(totemPieceId, "CheckpointId", "Id", checkpoint)
	world:GetComponent(totemPieceId, "Color", "X"):SetFloat3(colorX, colorY, colorZ)
	
end

CheckpointSystem.HasReachedFinish = function(self, entityId)
	
	local	playerId	= 	world:GetComponent(entityId, "PlayerEntityId", "Id"):GetInt()
	
	-- Take all cards from the player
	local newId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("TakeCardsFromPlayer", newId)
	world:GetComponent(newId, "TakeCardsFromPlayer", "Player"):SetInt(playerId)
	
	-- Take all steps from the player
	newId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("TakeCardStepsFromUnit", newId)
	world:GetComponent(newId, "TakeCardStepsFromUnit", "Unit"):SetInt(entityId)
	
	-- If the unit is not controlled by an AI
	if not world:EntityHasComponent(playerId, "AI") then
		--	Make the player a spectator
		world:CreateComponentAndAddTo("IsSpectator", playerId)
	else
		-- Else if the player is an AI, remove the unit.
		world:KillEntity(entityId)
		-- The "ai-player" will be removed in TakeCardsFromPlayerSystem when its cards have been removed.
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
				local R, G, B = world:GetComponent(entityId, "Color", 0):GetFloat3()
				self:AddTotemPiece(playerNum, targetCheckpoint, R, G, B)
				
				local	checkpointReached	=	world:CreateNewEntity()
				world:CreateComponentAndAddTo("CheckpointReached", checkpointReached)
				world:GetComponent(checkpointReached, "CheckpointReached", "CheckpointNumber"):SetInt(tempCheckpointId)
				world:GetComponent(checkpointReached, "CheckpointReached", "PlayerNumber"):SetInt(playerNum)
				if targetCheckpoint+1 > #allCheckpoints then
					self:HasReachedFinish(entityId)
				else
					self:SendInfoToClient(playerNum, targetCheckpoint+1)
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


CheckpointSystem.SendInfoToClient = function(self, player, nextCheckpoint)

	print("SendInfoToClient player " .. player)
	--	Check if the player is a player and not AI
	local	allPlayers	=	self:GetEntities("Player")
	local	playerId	=	-1
	for pId = 1, #allPlayers do
		
		if player == world:GetComponent(allPlayers[pId], "PlayerNumber", "Number"):GetInt() then
			playerId	=	allPlayers[pId]
			break
		end
		
	end
	if playerId == -1 then
		return
	end

	--	Get connection information
	local	IP		= 	world:GetComponent(playerId, "NetConnection", "IpAddress"):GetText()
	local	PORT	= 	world:GetComponent(playerId, "NetConnection", "Port"):GetInt()
	
    --	Checkpoint information
	local	X, Z			=	-1, -1
	local	tCheckpoints	=	self:GetEntities("Checkpoint")
	for tCheckId = 1, #tCheckpoints do
		if nextCheckpoint == world:GetComponent(tCheckpoints[tCheckId], "Checkpoint", "Number"):GetInt() then
			X, Z	=	world:GetComponent(tCheckpoints[tCheckId], "MapPosition", "X"):GetInt2()
			break
		end
	end
	
	
	print("Sending info about checkpoint at " .. X .. ", " .. Z .. " for player " .. player)
	local id = Net.StartPack("Client.NewTargetCheckpoint")
	Net.WriteInt(id, nextCheckpoint)
	Net.WriteInt(id, X)
	Net.WriteInt(id, Z)
	Net.Send(id, IP, PORT)
	
end


