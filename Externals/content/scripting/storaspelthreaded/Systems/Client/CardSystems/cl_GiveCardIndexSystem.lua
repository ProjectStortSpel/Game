GiveCardIndexSystem = System()

GiveCardIndexSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("GiveCardIndexSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Card", FilterType.Mandatory)
end

GiveCardIndexSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, newEntities)
	local cards = self:GetEntities()
	local indicies = {}
	for i = 1, #cards do
		if world:EntityHasComponent(cards[i], "CardIndex") then
			local index = world:GetComponent(cards[i], "CardIndex", "Index"):GetInt()
			indicies[index] = true
			print(index)
		end
	end
	for n = 1, #newEntities do
		local entityId = newEntities[n]
		local index = 1
		while indicies[index] do
			index = index + 1
		end
		world:CreateComponentAndAddTo("CardIndex", entityId)
		world:SetComponent(entityId, "CardIndex", "Index", index)
		indicies[index] = true
	end
end
