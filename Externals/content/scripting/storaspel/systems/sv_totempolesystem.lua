TotemPoleSystem = System()

TotemPoleSystem.Initialize = function(self)
	self:SetName("TotemPoleSystem System")

	self:AddComponentTypeToFilter("AddTotemPiece", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TotemPole", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Finishpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)

	print("TotemPoleSystem initialized!")
end

TotemPoleSystem.Update = function(self, dt)
end

TotemPoleSystem.CheckCheckPoints = function(self, targetCpId)
end

TotemPoleSystem.OnEntityAdded = function(self, entity)

	print("TotemPoleSystem.OnEntityAdded")

	if world:EntityHasComponent(entity, "AddTotemPiece") then
	
		print("OnEntityAdded New AddTotemPiece")
	
		local playerNum 	= world:GetComponent(entity, "PlayerNumber", 0):GetInt()
		
		local targetCpId	= world:GetComponent(entity, "CheckpointId", 0):GetInt()
		local totemPoles 	= self:GetEntities("TotemPole")
		local totemNr		= -1
		
		for i = 1, #totemPoles do
		
			local totemCpId = world:GetComponent(totemPoles[i], "CheckpointId", 0):GetInt()
			if totemCpId == targetCpId then
				totemNr = i
				break
			end
		
		end
		
		if totemNr == -1 then
		
			print("No totemCpId match targetCpId: " .. targetCpId)
		
			local newTotemPole = world:CreateNewEntity()
			world:CreateComponentAndAddTo("TotemPole", newTotemPole)
			world:CreateComponentAndAddTo("CheckpointId", newTotemPole)
			
			world:SetComponent(newTotemPole, "TotemPole", "Height", 0)
			world:SetComponent(newTotemPole, "CheckpointId", "Id", targetCpId)
		
			totemNr = targetCpId
		
		end
		
		--self.CheckCheckPoints(self, totemNr

	end
end