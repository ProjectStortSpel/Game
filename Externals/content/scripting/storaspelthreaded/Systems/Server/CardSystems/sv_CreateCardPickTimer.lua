CreateCardPickTimer = System()
CreateCardPickTimer.TimeLimit = 300.0

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
		
		--	Send time amount in message to all players
	end
end