ClearServerSelectedSystem = System()

ClearServerSelectedSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ClearServerSelectedSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ServerSelectedCard", FilterType.RequiresOneOf)
end

ClearServerSelectedSystem.ClearSelectedCards = function(self)
	
	local	selectedCards	=	self:GetEntities("ServerSelectedCard")
	for i = 1, #selectedCards do
		world:GetComponent(selectedCards[i], "ServerSelectedCard", "Index"):SetInt(0)
		world:RemoveComponentFrom("ServerSelectedCard", selectedCards[i])
	end

end

ClearServerSelectedSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "NewRound") then
			self:ClearSelectedCards()
		end
	end
end
