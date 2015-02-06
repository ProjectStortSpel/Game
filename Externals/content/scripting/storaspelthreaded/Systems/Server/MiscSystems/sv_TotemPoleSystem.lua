TotemPoleSystem = System()
TotemPoleSystem.MapCenterX = 0
TotemPoleSystem.MapCenterZ = 0


TotemPoleSystem.Initialize = function(self)
	--	Set Name
	self:SetName("TotemPoleSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("AddTotemPiece", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TotemPole", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Finishpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
end

TotemPoleSystem.AddTotemPiece = function(self, currentPlayerNumber, height, X, Z)

	local totemPiece = world:CreateNewEntity("TotemPiece")
	local rotation 	= world:GetComponent(totemPiece, "Rotation", 0)
	local position 	= world:GetComponent(totemPiece, "Position", 0)
	local scale		= world:GetComponent(totemPiece, "Scale", 0)
	
	local lookAtX = X
	local lookAtZ = Z
	
	local tempMoveX = self.MapCenterX - X
	local tempMoveZ = self.MapCenterZ - Z

	
	local totemAngle = math.pi/2
	if tempMoveX ~= 0 then
		totemAngle = totemAngle - math.atan(tempMoveZ/tempMoveX)
	end
	
	
	-- Rotation
	rotation:SetFloat3(0, totemAngle, 0)
	
	-- Position
	local offsetX = 0.25;
	local offsetZ = 0.25;
	
	if tempMoveX > 0 then
		offsetX = offsetX * -1
	else
		offsetX = offsetX * 1
	end
	
	if tempMoveZ > 0 then
		offsetZ = offsetZ * -1
	else
		offsetZ = offsetZ * 1
	end
	
	
	

	position:SetFloat3(X + offsetX, 0.95 + height*0.27, Z + offsetZ)
	-- Scale
	scale:SetFloat3(1,1,1)
	world:SetComponent(totemPiece, "Model", "ModelName", "totem" .. currentPlayerNumber)
	world:SetComponent(totemPiece, "Model", "ModelPath", "totem")
	world:SetComponent(totemPiece, "Model", "RenderType", 0)
end

TotemPoleSystem.CreateTotemPole = function(self, totemCheckpointNumber, tX, tZ)
	--	Create the first totempole if none was found 
	local newTotemPole = world:CreateNewEntity()
	world:CreateComponentAndAddTo("TotemPole", newTotemPole)
	world:CreateComponentAndAddTo("CheckpointId", newTotemPole)
	
	world:SetComponent(newTotemPole, "TotemPole", "Height", 0)
	world:SetComponent(newTotemPole, "CheckpointId", "Id", totemCheckpointNumber)
end

TotemPoleSystem.CheckCheckPoints = function(self, targetCpId, totemId, playerNum)

	local checkPoints = self:GetEntities("Checkpoint")
	for i = 1, #checkPoints do
	
		local cpId = world:GetComponent(checkPoints[i], "Checkpoint", "Number"):GetInt()
		
		-- Found the correct checkpoint
		if cpId == targetCpId then
		
			-- Get the checkPoints' position
			local cpPosX, cpPosZ = world:GetComponent(checkPoints[i], "MapPosition", 0):GetInt2()
			-- Get the height of the current totempole
			local height = world:GetComponent(totemId, "TotemPole", "Height"):GetInt()
			-- Add a new piece
			self.AddTotemPiece(self, playerNum, height, cpPosX, cpPosZ)
			world:SetComponent(totemId, "TotemPole", "Height", height + 1)
			return true
		end
		
	end
	
	-- No checkpoint found
	return false
	
end


TotemPoleSystem.CheckFinishPoints = function(self, targetFpId, totemId, playerNum)

	-- get all finishPoints and go through them
	local finishPoints = self:GetEntities("Finishpoint")
	for i = 1, #finishPoints do
	
		-- If we found the correct finishpoint
		if finishPoints[i] == targetFpId then
		
			-- Get the finishpoints' position
			local cpPosX, cpPosZ = world:GetComponent(finishPoints[i], "MapPosition", 0):GetInt2()
			-- Get the height of the current totempole
			local height = world:GetComponent(totemId, "TotemPole", "Height"):GetInt()
			-- Add the new piece
			self.AddTotemPiece(self, playerNum, height, cpPosX, cpPosZ)
			world:SetComponent(totemId, "TotemPole", "Height", height + 1)
			return true
		end
		
	end

	-- No finishpoint found
	return false
	
end

TotemPoleSystem.CheckAddTotemPiece = function(self, entityId)

	--	Get Player and Checkpoint number
	local	playerNumber		=	world:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
	local	targetCheckpointId	=	world:GetComponent(entityId, "CheckpointId", "Id"):GetInt()
	
	--	Get the actual totem pole (if there is one)
	local	totemPoles	=	self:GetEntities("TotemPole")
	local	totemPoleId	=	-1
	
	for iTotem = 1, #totemPoles do
		local	totemCheckpointId	=	world:GetComponent(totemPoles[iTotem], "CheckpointId", 0):GetInt()
		if totemCheckpointId == targetCheckpointId then
			totemPoleId = totemPoles[iTotem]
			break
		end
	end
	
	if totemPoleId == -1 then
		print("Error: TotemPoleId is -1 in TotemPoleSystem. TargetCheckpointId is: " .. targetCheckpointId)
	end
	
	-- Check if the totempole is for a checkpoint
	local success = self:CheckCheckPoints(targetCheckpointId, totemPoleId, playerNumber)
	-- If not, it's for the finishpoint
	if not success then
		self:CheckFinishPoints(targetCheckpointId, totemPoleId, playerNumber)	
	end
end

TotemPoleSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, addedEntities)

	for n = 1, #addedEntities do
		--	Get entity ID
		local newEntity = addedEntities[n]
		
		--	Add Totem Piece logic
		if world:EntityHasComponent(newEntity, "AddTotemPiece") then
			self:CheckAddTotemPiece(newEntity)
		end
		
		--	New Checkpoint is added
		if world:EntityHasComponent(newEntity, "Checkpoint") then
			local tX, tZ = world:GetComponent(newEntity, "MapPosition", 0):GetInt2()
			local checkpointId = world:GetComponent(newEntity, "Checkpoint", "Number"):GetInt()
			self:CreateTotemPole(checkpointId, tX, tZ)
			
			print("Checkpoint added at " .. tX .. ", " .. tZ .. " with number " .. checkpointId)
		end
		
		--	Get Center and also set up all angles on TotemPoles
		if world:EntityHasComponent(newEntity, "MapSpecs") then
			local tX, tZ = world:GetComponent(newEntity, "MapSpecs", "SizeX"):GetInt2()
			
			print("Map Size!" .. tX .. ", " .. tZ)
			
			self.MapCenterX = tX * 0.5
			self.MapCenterZ = tZ * 0.5
		end
	
	
	end 
end