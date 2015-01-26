

AddCardPickTimer = System()

AddCardPickTimer.Initialize = function(self)
	self:SetName("AddCardPickTimer")

	self:AddComponentTypeToFilter("PickingPhaseTimer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AddToPickingPhaseTimer", FilterType.RequiresOneOf)
end

AddCardPickTimer.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent(entity, "AddToPickingPhaseTimer") then

		local Timers = self:GetEntities("PickingPhaseTimer")
		
		if #Timers > 0 then

			local deltaTime = world:GetComponent(entity, "AddToPickingPhaseTimer", "Amount"):GetFloat()
			local oldTime = world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer"):GetFloat()
			local newTime = oldTime + deltaTime
			
			if newTime < 10 and oldTime > 10 then
				newTime = 10
			end
			
			world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer"):SetFloat(newTime)
			
			local id = Net.StartPack("Client.SendPickingPhaseTimer")
			Net.WriteFloat(id, newTime)
			Net.Broadcast(id)
		end
		
		world:KillEntity(entity)
	end
end

SetCardPickTimer = System()

SetCardPickTimer.Initialize = function(self)
	self:SetName("SetCardPickTimer")

	self:AddComponentTypeToFilter("PickingPhaseTimer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SetPickingPhaseTimer", FilterType.RequiresOneOf)
end

SetCardPickTimer.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent(entity, "SetPickingPhaseTimer") then
	
		local Timers = self:GetEntities("PickingPhaseTimer")
		
		if #Timers > 0 then
		
			local newTime = world:GetComponent(entity, "SetPickingPhaseTimer", "Amount"):GetFloat()
			world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer"):SetFloat(newTime)
			
			local id = Net.StartPack("Client.SendPickingPhaseTimer")
			Net.WriteFloat(id, newTime)
			Net.Broadcast(id)
			
		end
		
		world:KillEntity(entity)
	end
end





UpdateCardPickTimer = System()
UpdateCardPickTimer.TimeLimit = 30.0

UpdateCardPickTimer.Initialize = function(self)
	self:SetName("UpdateCardPickTimer")

	self:AddComponentTypeToFilter("PickingPhaseTimer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("OnPickingPhase", FilterType.RequiresOneOf)
end

UpdateCardPickTimer.Update = function(self, dt)

	local Timers = self:GetEntities("PickingPhaseTimer")
	if #Timers > 0 then
		
		local Timer = world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer")
		local newTime = Timer:GetFloat() - dt
		Timer:SetFloat(newTime)
		
		if newTime < 0 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("AutoPickCards", id)
			world:KillEntity(Timers[1])
		end
	end

end

UpdateCardPickTimer.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent(entity, "OnPickingPhase") then
		local Timers = self:GetEntities("PickingPhaseTimer")
		print("DELETE TIMERS!!! -> " .. #Timers)
		for i = 1, #Timers do
			world:KillEntity(Timers[i])
		end
		
		world:KillEntity(entity)
		
		
		local newId = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PickingPhaseTimer", newId)
		world:GetComponent(newId, "PickingPhaseTimer", "Timer"):SetFloat(self.TimeLimit)
		
		local id = Net.StartPack("Client.SendPickingPhaseTimer")
		Net.WriteFloat(id, self.TimeLimit)
		Net.Broadcast(id)
	end

end
