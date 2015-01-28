SortCardIndexSystem = System()

SortCardIndexSystem.Initialize = function(self)
		--	Set Name
	self:SetName("SortCardIndexSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
end

SortCardIndexSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		local index = world:GetComponent(entityId, "CardIndex", "Index"):GetInt()

		local cards = self:GetEntities()

		for i = 1, #cards do
			local index2 = world:GetComponent(cards[i], "CardIndex", "Index"):GetInt()
			
			if index2 > index then
				world:SetComponent(cards[i], "CardIndex", "Index", index2 - 1)
			end

		end
	end
end