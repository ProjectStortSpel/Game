TotemPoleSystem = System()

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
end


TotemPoleSystem.AddTotemPiece = function(self, currentPlayerNumber, height, X, Z)

	local totemPiece = world:CreateNewEntity("TotemPiece")
	local rotation 	= world:GetComponent(totemPiece, "Rotation", 0)
	local position 	= world:GetComponent(totemPiece, "Position", 0)
	local scale		= world:GetComponent(totemPiece, "Scale", 0)
	-- Rotation
	rotation:SetFloat3(0, 0, 0)	
	-- Position
	local offsetX = 0.10;
	local offsetZ = 0.10;
	position:SetFloat3(X + offsetX, 0.95 + height*0.27, Z + offsetZ)
	-- Scale
	scale:SetFloat3(1,1,1)
	world:SetComponent(totemPiece, "Model", "ModelName", "totem" .. currentPlayerNumber)
	world:SetComponent(totemPiece, "Model", "ModelPath", "totem")
	world:SetComponent(totemPiece, "Model", "RenderType", 0)
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

TotemPoleSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		-- If the entity is a "AddTotemPiece"
		if world:EntityHasComponent(entity, "AddTotemPiece") then
		
			-- Get the playerNumber and what checkpoint the piece should go with
			local playerNum 	= world:GetComponent(entity, "PlayerNumber", 0):GetInt()
			local targetCpId	= world:GetComponent(entity, "CheckpointId", 0):GetInt()
			-- Get all totempoles
			local totemPoles 	= self:GetEntities("TotemPole")
			local totemId		= -1
			
			-- Go through all checkPoints and check if its id match with the totempole
			for i = 1, #totemPoles do
				
				local totemCpId = world:GetComponent(totemPoles[i], "CheckpointId", 0):GetInt()
				if totemCpId == targetCpId then
					totemId = totemPoles[i]
					break
				end
			
			end

			-- If no match was found there does not exist an totempole for that checkpoint yet
			-- Create it
			if totemId == -1 then
				print("TOTEMPOLE NOT FOUND! CREATE ONE!")
				local newTotemPole = world:CreateNewEntity()
				world:CreateComponentAndAddTo("TotemPole", newTotemPole)
				world:CreateComponentAndAddTo("CheckpointId", newTotemPole)
				
				world:SetComponent(newTotemPole, "TotemPole", "Height", 0)
				world:SetComponent(newTotemPole, "CheckpointId", "Id", targetCpId)
			
				totemId = newTotemPole
			
			end
			
			-- Check if the totempole is for a checkpoint
			local success = self:CheckCheckPoints(targetCpId, totemId, playerNum)
			-- If not, it's for the finishpoint
			if not success then
				self:CheckFinishPoints(targetCpId, totemId, playerNum)	
			end
			
		end
	end
end