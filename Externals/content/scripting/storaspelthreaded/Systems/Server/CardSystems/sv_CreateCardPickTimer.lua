CreateCardPickTimer = System()
CreateCardPickTimer.TimeLimit = 60.0

CreateCardPickTimer.Initialize = function(self)
	--	Set Name
	self:SetName("CreateCardPickTimer")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("OnPickingPhase",	FilterType.Mandatory)
end

CreateCardPickTimer.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local newId = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PickingPhaseTimer", newId)
		world:GetComponent(newId, "PickingPhaseTimer", "Timer"):SetFloat(self.TimeLimit)
		
		print("Creating timer with " .. self.TimeLimit)
		
		local id = Net.StartPack("Client.SendPickingPhaseTimer")
		Net.WriteFloat(id, self.TimeLimit)
		Net.Broadcast(id)
		
		world:KillEntity(entity)
	end
end