
SetCardPickTimer = System()

SetCardPickTimer.Initialize = function(self)
	--	Set Name
	self:SetName("SetCardPickTimer")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PickingPhaseTimer", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SetPickingPhaseTimer",	FilterType.RequiresOneOf)
end

SetCardPickTimer.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "SetPickingPhaseTimer") then
		
			local Timers = self:GetEntities("PickingPhaseTimer")
			
			if #Timers > 0 then
				print("SetCardPickTimer says hi!")
				local newTime = world:GetComponent(entity, "SetPickingPhaseTimer", "Amount"):GetFloat()
				world:GetComponent(Timers[1], "PickingPhaseTimer", "Timer"):SetFloat(newTime)
				
				local id = Net.StartPack("Client.SendPickingPhaseTimer")
				Net.WriteFloat(id, newTime)
				Net.Broadcast(id)
				
			end
			
			world:KillEntity(entity)
		end
	end
end