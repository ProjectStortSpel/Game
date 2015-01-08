SortCardIndexSystem = System()

SortCardIndexSystem.Update = function(self, dt)

end

SortCardIndexSystem.Initialize = function(self)
	self:SetName("SortCardIndexSystem")
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)

	print("SortCardIndexSystem initialized!")
end

SortCardIndexSystem.OnEntityRemoved = function(self, entityId)
	
	local index = world:GetComponent(entityId, "CardIndex", "Index"):GetInt()

	local cards = self:GetEntities()

	for i = 1, #cards do
		local index2 = world:GetComponent(cards[i], "CardIndex", "Index"):GetInt()
		
		if index2 > index then
			world:SetComponent(cards[i], "CardIndex", "Index", index2 - 1)
		end

	end

end