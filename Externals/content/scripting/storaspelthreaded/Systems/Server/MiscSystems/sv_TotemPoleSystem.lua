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


TotemPoleSystem.AddTotemPiece = function(self, playerNumber, height, X, Z)

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
	
	world:SetComponent(totemPiece, "Model", "ModelName", "totem" .. playerNumber)
	world:SetComponent(totemPiece, "Model", "ModelPath", "totem")
	world:SetComponent(totemPiece, "Model", "RenderType", 0)
end

TotemPoleSystem.CheckCheckPoints = function(self, targetCpId, totemId, playerNum)

	local checkPoints = self:GetEntities("Checkpoint")
		for i = 1, #checkPoints do
		
		local cpId = world:GetComponent(checkPoints[i], "Checkpoint", "Number"):GetInt()
			
		-- If we found the correct checkpoint
			if cpId == targetCpId then
			
				local cpPosX, cpPosZ = world:GetComponent(checkPoints[i], "MapPosition", 0):GetInt2()
				local height = world:GetComponent(totemId, "TotemPole", "Height"):GetInt()
				print("Height: " .. height)
				
				print("Found the correct checkpoint")
				self.AddTotemPiece(self, playerNum, height, cpPosX, cpPosZ)
				
				world:SetComponent(totemId, "TotemPole", "Height", height + 1)
				
				return true
			end
			
	end

	print("No Check found")
	return false
	
end


TotemPoleSystem.CheckFinishPoints = function(self, targetFpId, totemId, playerNum)

	local finishPoints = self:GetEntities("Finishpoint")
		for i = 1, #finishPoints do
		
		-- If we found the correct checkpoint
			if finishPoints[i] == targetFpId then
			
				local cpPosX, cpPosZ = world:GetComponent(finishPoints[i], "MapPosition", 0):GetInt2()
				local height = world:GetComponent(totemId, "TotemPole", "Height"):GetInt()
				print("Height: " .. height)
				
				print("Found the correct finishPoint")
				self.AddTotemPiece(self, playerNum, height, cpPosX, cpPosZ)
				
				world:SetComponent(totemId, "TotemPole", "Height", height + 1)
				
				return true
			end
			
	end

	print("No Check found")
	return false
	
end

TotemPoleSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		--print("TotemPoleSystem.OnEntityAdded")

		if world:EntityHasComponent(entity, "AddTotemPiece") then
		
			--print("OnEntityAdded New AddTotemPiece")
		
			local playerNum 	= world:GetComponent(entity, "PlayerNumber", 0):GetInt()
			
			local targetCpId	= world:GetComponent(entity, "CheckpointId", 0):GetInt()
			local totemPoles 	= self:GetEntities("TotemPole")
			local totemId		= -1
			

			
			for i = 1, #totemPoles do
			
				local totemCpId = world:GetComponent(totemPoles[i], "CheckpointId", 0):GetInt()
				if totemCpId == targetCpId then
					totemId = totemPoles[i]
					break
				end
			
			end
			
			
			if totemId == -1 then
			
				--print("No totemCpId match targetCpId: " .. targetCpId)
			
				local newTotemPole = world:CreateNewEntity()
				world:CreateComponentAndAddTo("TotemPole", newTotemPole)
				world:CreateComponentAndAddTo("CheckpointId", newTotemPole)
				
				world:SetComponent(newTotemPole, "TotemPole", "Height", 0)
				world:SetComponent(newTotemPole, "CheckpointId", "Id", targetCpId)
			
				totemId = newTotemPole
			
			end

			--print("targetCpId: " .. targetCpId)
			--print("totemPoles: " .. totemPoles)
			--print("totemId: " .. totemId)
			--print("playerNum: " .. playerNum)
			
			local success = self.CheckCheckPoints(self, targetCpId, totemId, playerNum)
			if not success then
				--print("No checkpoint found")
				
				self.CheckFinishPoints(self, targetCpId, totemId, playerNum)
				
			end
			
		end
	end
end