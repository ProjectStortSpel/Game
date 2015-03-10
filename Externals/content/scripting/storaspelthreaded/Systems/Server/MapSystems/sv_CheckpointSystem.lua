CheckpointSystem = System()
CheckpointSystem.MapCenterX	=	0
CheckpointSystem.MapCenterZ	=	0
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
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
end

CheckpointSystem.AddTotemPiece = function(self, playerNumber, checkpoint, colorX, colorY, colorZ)

	local totemPieceId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
	world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
	world:CreateComponentAndAddTo("CheckpointId", totemPieceId)
	world:CreateComponentAndAddTo("Color", totemPieceId)
	
	world:GetComponent(totemPieceId, "PlayerNumber", "Number"):SetInt(playerNumber)
	world:GetComponent(totemPieceId, "CheckpointId", "Id"):SetInt(checkpoint)
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
		
		local counterEntities = self:GetEntities("PlayerCounter")
		local specCounterComp = world:GetComponent(counterEntities[1], "PlayerCounter", "Spectators")
		local number = specCounterComp:GetInt(0)
		number = number + 1
		world:SetComponent(counterEntities[1], "PlayerCounter", "Spectators", number)
		
		local playerCounterComp = world:GetComponent(counterEntities[1], "PlayerCounter", "Players")
		number = playerCounterComp:GetInt(0)
		number = number - 1
		world:SetComponent(counterEntities[1], "PlayerCounter", "Players", number)
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

CheckpointSystem.EntitiesAdded = function(self, dt, newEntities)

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

		if world:EntityHasComponent(entity, "GameRunning") then
			
			local	allPlayers	=	self:GetEntities("Player")
			for tPlayer = 1, #allPlayers do
			
				if world:EntityHasComponent(allPlayers[tPlayer], "PlayerNumber") then
					local	playerNum	=	world:GetComponent(allPlayers[tPlayer], "PlayerNumber", 0):GetInt()
					self:SendInfoToClient(playerNum, 1)
				end
			end
		end
		
		--	Get Center and also set up all angles on TotemPoles
		if world:EntityHasComponent(entity, "MapSpecs") then
			local tX, tZ = world:GetComponent(entity, "MapSpecs", "SizeX"):GetInt2()
			
			self.MapCenterX = tX * 0.5
			self.MapCenterZ = tZ * 0.5
		end
		
	end
	
end


CheckpointSystem.SendInfoToClient = function(self, player, nextCheckpoint)

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
	
	
	local tempMoveX = self.MapCenterX - X
	local tempMoveZ = self.MapCenterZ - Z
	-- Position
	local offsetX = 0.35;
	local offsetZ = 0.35;
	
	if tempMoveX < 0 then
		offsetX = offsetX * -1
	else
		offsetX = offsetX * 1
	end
	
	if tempMoveZ < 0 then
		offsetZ = offsetZ * -1
	else
		offsetZ = offsetZ * 1
	end
	
	
	local id = Net.StartPack("Client.NewTargetCheckpoint")
	Net.WriteInt(id, nextCheckpoint)
	Net.WriteFloat(id, X+offsetX)
	Net.WriteFloat(id, Z+offsetZ)
	Net.Send(id, IP, PORT)
	
end


