
UpdateCardPickTimer = System()
UpdateCardPickTimer.Initialize = function(self)
	--	Set Name
	self:SetName("UpdateCardPickTimer")
	
	--	Toggle EntitiesAdded and Update
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("PickingPhaseTimer", 	FilterType.RequiresOneOf)
	--self:AddComponentTypeToFilter("OnPickingPhase", 	FilterType.RequiresOneOf)
end

UpdateCardPickTimer.Update = function(self, dt)

	--local Timers = self:GetEntities("PickingPhaseTimer")
	local Timers = self:GetEntities()
	if #Timers > 0 then
		
		local Timer = world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer")
		local newTime = Timer:GetFloat() - dt
		Timer:SetFloat(newTime)
		
		if newTime < 0 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("AutoPickCards", id)
			world:KillEntity(Timers[1])
			
			local audioId = Net.StartPack("Client.FadeOutSound")
			Net.WriteString(audioId, "PickingMusic")
			Net.WriteInt(audioId, 200)
			Net.Broadcast(audioId)
		end
	end

end

UpdateCardPickTimer.EntitiesAdded = function(self, dt, entities)

	local newTimer = entities[1]
	local allTimers = self:GetEntities()
	
	if #allTimers > 1 then
		for n = 1, #allTimers do
			if not allTimers[n] == newTimer then
				world:KillEntity(allTimers[n])
			end
		end
	end
end