FadeTreeSystem = System()

FadeTreeSystem.Initialize = function(self)
	--	Set Name
	self:SetName("FadeTreeSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("OnPickingPhase", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	
end

FadeTreeSystem.EntitiesAdded = function(self, dt, addedEntities)
	
	for n = 1, #addedEntities do
		--	Get entity ID
		local newEntity = addedEntities[n]
		
	end 
	
	Net.Broadcast(Net.StartPack("Client.ToggleTrees"))
	
end