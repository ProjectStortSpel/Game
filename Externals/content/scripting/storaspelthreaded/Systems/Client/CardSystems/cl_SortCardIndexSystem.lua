SortCardIndexSystem = System()

SortCardIndexSystem.Initialize = function(self)
		--	Set Name
	self:SetName("SortCardIndexSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
end

SortCardIndexSystem.EntitiesRemoved = function(self, dt, entities)
	local cards = self:GetEntities()
	for i = 1, #cards do
		world:SetComponent(cards[i], "CardIndex", "Index", i)
	end
end