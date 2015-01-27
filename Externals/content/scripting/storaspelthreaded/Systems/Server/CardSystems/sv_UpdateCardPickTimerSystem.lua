
UpdateCardPickTimer = System()
UpdateCardPickTimer.Initialize = function(self)
	--	Set Name
	self:SetName("UpdateCardPickTimer")
	
	--	Toggle EntitiesAdded and Update
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("PickingPhaseTimer", 	FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("OnPickingPhase", 	FilterType.RequiresOneOf)
end

UpdateCardPickTimer.Update = function(self, dt, taskIndex, taskCount)

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

UpdateCardPickTimer.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "OnPickingPhase") then
			local Timers = self:GetEntities("PickingPhaseTimer")
			print("DELETE TIMERS!!! -> " .. #Timers)
			for i = 1, #Timers do
				world:KillEntity(Timers[i])
			end
			
			world:KillEntity(entity)
		end
		
		--	Send time amount in message to all players
	end
end