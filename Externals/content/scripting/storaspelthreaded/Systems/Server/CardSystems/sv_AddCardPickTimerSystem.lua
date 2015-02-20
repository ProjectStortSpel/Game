
AddCardPickTimer = System()

AddCardPickTimer.Initialize = function(self)
	--	Set Name
	self:SetName("AddCardPickTimer")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set filter
	self:AddComponentTypeToFilter("PickingPhaseTimer", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AddToPickingPhaseTimer",	FilterType.RequiresOneOf)
end

AddCardPickTimer.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "AddToPickingPhaseTimer") then
		
			local Timers = self:GetEntities("PickingPhaseTimer")
			
			if #Timers > 0 then
				local deltaTime = world:GetComponent(entity, "AddToPickingPhaseTimer", "Amount"):GetFloat()
				local oldTime = world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer"):GetFloat()
				local newTime = oldTime + deltaTime
				
				if newTime < 10 and oldTime > 10 then
					newTime = 10
				elseif oldTime < 10 then
					newTime = oldTime
				end
				
				world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer"):SetFloat(newTime)
				
				local id = Net.StartPack("Client.SendPickingPhaseTimer")
				Net.WriteFloat(id, newTime)
				Net.Broadcast(id)
			end
			
			world:KillEntity(entity)
		end
	end
end
